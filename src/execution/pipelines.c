/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	execute_child_process(t_pipe_context *ctx)
{
	setup_pipe_redirections(ctx->pipes, ctx->cmd_index, ctx->cmd_count);
	close_all_pipes(ctx->pipes, ctx->cmd_count - 1);
	if (setup_redirections(&ctx->cmds[ctx->cmd_index]) == -1)
		exit(1);
	if (is_builtin(ctx->cmds[ctx->cmd_index].cmds[0]))
	{
		exit(execute_builtin(ctx->shell, &ctx->cmds[ctx->cmd_index]));
	}
	else
	{
		exit(execute_external_command(ctx->shell, &ctx->cmds[ctx->cmd_index]));
	}
}

static int	setup_pipeline_resources(t_pipe_context *ctx)
{
	ctx->pipes = create_pipes(ctx->cmd_count - 1);
	if (!ctx->pipes)
		return (1);
	return (0);
}

static pid_t	*allocate_pids(int cmd_count, int **pipes)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		free_pipes(pipes, cmd_count - 1);
		return (NULL);
	}
	return (pids);
}

static int	fork_pipeline_processes(t_pipe_context *ctx, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < ctx->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			ctx->cmd_index = i;
			execute_child_process(ctx);
		}
		else if (pids[i] == -1)
		{
			perror("fork");
			free_pipes(ctx->pipes, ctx->cmd_count - 1);
			free(pids);
			return (1);
		}
		i++;
	}
	return (0);
}

int	execute_pipeline(t_shell *shell, t_cmds *cmds, int cmd_count)
{
	t_pipe_context	ctx;
	pid_t			*pids;
	int				status;

	if (cmd_count == 1)
		return (execute_single_command_with_redirections(shell, &cmds[0]));
	ctx.shell = shell;
	ctx.cmds = cmds;
	ctx.cmd_count = cmd_count;
	if (setup_pipeline_resources(&ctx) != 0)
		return (1);
	pids = allocate_pids(cmd_count, ctx.pipes);
	if (!pids)
		return (1);
	if (fork_pipeline_processes(&ctx, pids) != 0)
		return (1);
	close_all_pipes(ctx.pipes, cmd_count - 1);
	status = wait_for_children(pids, cmd_count);
	free_pipes(ctx.pipes, cmd_count - 1);
	free(pids);
	return (status);
}
