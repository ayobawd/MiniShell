/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

void	ms_signals_child_default(void);
int		ms_apply_redirs(t_redir *rlist, int fds[2]);
int		ms_run_builtin(char **argv, t_env **env, bool in_parent);
char	*ms_resolve_path(const char *cmd, t_env *env);
char	**ms_env_to_envp(t_env *env);

static void	exec_with_path(t_cmd *cmd, char **envp, char *path)
{
	if (path)
	{
		execve(path, cmd->argv, envp);
		free(path);
	}
	else if (ft_strchr(cmd->argv[0], '/'))
		execve(cmd->argv[0], cmd->argv, envp);
	handle_exec_error(cmd->argv[0]);
}



static void	child_exec(t_cmd *cmd, t_env **env, t_child_context *ctx)
{
	int		fds[2];
	char	**envp;
	char	*path;

	ms_signals_child_default();
	setup_child_pipes(ctx->i, ctx->n, ctx->pipes, fds);
	if (ms_apply_redirs(cmd->redirs, fds) != 0)
		exit(1);
	close_pipes_all(ctx->n - 1, ctx->pipes);
	if (cmd->is_builtin)
		exit(ms_run_builtin(cmd->argv, env, false));
	path = ms_resolve_path(cmd->argv[0], *env);
	envp = ms_env_to_envp(*env);
	if (!cmd->argv[0] || !cmd->argv[0][0])
		exit(0);
	exec_with_path(cmd, envp, path);
}

static int	init_pipes(int n, int (**pipes)[2])
{
	if (n > 1)
	{
		*pipes = (int (*)[2])malloc(sizeof(int[2]) * (n - 1));
		if (!*pipes)
			return (1);
		if (open_pipes(n - 1, *pipes) != 0)
		{
			free(*pipes);
			return (1);
		}
	}
	return (0);
}

static int	exec_fork_loop(t_cmd *first, t_env **env, int n, int (*pipes)[2])
{
	t_cmd				*cur;
	int					i;
	int					status;
	int					last_pid;
	t_child_context		ctx;

	ctx.pipes = pipes;
	ctx.n = n;
	i = 0;
	cur = first;
	last_pid = -1;
	while (cur)
	{
		ctx.i = i;
		status = fork();
		if (status == 0)
			child_exec(cur, env, &ctx);
		else if (status < 0)
			return (-1);
		else
		{
			last_pid = status;
			if (i > 0)
			{
				close(pipes[i - 1][0]);
				close(pipes[i - 1][1]);
			}
		}
		i++;
		cur = cur->next;
	}
	return (last_pid);
}

int	ms_exec_pipeline(t_cmd *first, t_env **env)
{
	int		n;
	t_cmd	*cur;
	int		(*pipes)[2];
	int		last_pid;
	int		status;

	n = 0;
	cur = first;
	while (cur && ++n)
		cur = cur->next;
	pipes = NULL;
	if (init_pipes(n, &pipes) != 0)
		return (1);
	last_pid = exec_fork_loop(first, env, n, pipes);
	if (pipes)
	{
		if (n > 1)
		{
			close(pipes[n - 2][0]);
			close(pipes[n - 2][1]);
		}
		free(pipes);
	}
	if (last_pid == -1)
		return (1);
	status = wait_children(last_pid, n);
	return (status);
}
