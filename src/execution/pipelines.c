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

static int	execute_builtin_with_redirect(t_shell *shell, t_cmds *cmd,
		int saved_stdin, int saved_stdout)
{
	int	status;

	if (setup_redirections(cmd) == -1)
	{
		restore_fds(saved_stdin, saved_stdout);
		return (1);
	}
	status = execute_builtin(shell, cmd);
	restore_fds(saved_stdin, saved_stdout);
	return (status);
}

static int	fork_and_execute_command(t_shell *shell, t_cmds *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (setup_redirections(cmd) == -1)
			exit(1);
		if (is_builtin(cmd->cmds[0]))
			exit(execute_builtin(shell, cmd));
		else
			exit(execute_external_command(shell, cmd));
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else
			status = 1;
	}
	else
	{
		perror("fork");
		status = 1;
	}
	return (status);
}

int	execute_single_command_with_redirections(t_shell *shell, t_cmds *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (is_builtin(cmd->cmds[0]) && !should_fork_builtin(cmd))
		status = execute_builtin_with_redirect(shell, cmd,
				saved_stdin, saved_stdout);
	else
	{
		status = fork_and_execute_command(shell, cmd);
		restore_fds(saved_stdin, saved_stdout);
	}
	return (status);
}

int	**create_pipes(int num_pipes)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * num_pipes);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < num_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			free_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count)
{
	if (cmd_index > 0)
	{
		dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
	}
	if (cmd_index < cmd_count - 1)
	{
		dup2(pipes[cmd_index][1], STDOUT_FILENO);
	}
}

void	close_all_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	free_pipes(int **pipes, int num_pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < num_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	wait_for_children(pid_t *pids, int count)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else
			last_status = 1;
		i++;
	}
	return (last_status);
}

int	should_fork_builtin(t_cmds *cmd)
{
	if (ft_strncmp(cmd->cmds[0], "cd", 3) == 0)
		return (0);
	if (ft_strncmp(cmd->cmds[0], "export", 7) == 0)
		return (0);
	if (ft_strncmp(cmd->cmds[0], "unset", 6) == 0)
		return (0);
	if (ft_strncmp(cmd->cmds[0], "exit", 5) == 0)
		return (0);
	return (1);
}

void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}