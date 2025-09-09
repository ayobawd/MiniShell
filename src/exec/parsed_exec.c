/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

/* Forward declarations */
void	ms_signals_child_default(void);
int		ms_run_builtin(char **argv, t_env **env, bool in_parent);
char	*ms_resolve_path(const char *cmd, t_env *env);
char	**ms_env_to_envp(t_env *env);
int		ms_is_builtin(const char *name);
void	close_pipes_all(int n, int pipes[][2]);
int		wait_children(int last_pid, int count);
t_redir	*convert_redirections(t_cmds *cmd);
void	free_redirs(t_redir *redirs);
void	setup_child_fds(int i, int count, int pipes[][2], int fds[2]);
void	handle_exec_error(char *cmd_name);
void	exec_with_path(t_cmds *cmd, char **envp, char *path);
bool	should_run_in_parent(t_cmds *cmd);
void	setup_child_exec_env(t_cmds *cmd, int i, int count, int pipes[][2]);
void	do_exec_cmd(t_cmds *cmd, t_env **env);

/* Create pipes for pipeline */
static int	create_pipes(int count, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < count - 1)
	{
		if (pipe(pipes[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

/* Execute single builtin in parent */
static int	exec_single_builtin(t_cmds *cmd, t_env **env)
{
	t_redir	*redirs;
	int		fds[2];
	int		status;

	fds[0] = -1;
	fds[1] = -1;
	redirs = convert_redirections(cmd);
	if (ms_apply_redirs(redirs, fds) != 0)
	{
		free_redirs(redirs);
		return (1);
	}
	status = ms_run_builtin(cmd->cmds, env, true);
	free_redirs(redirs);
	return (status);
}

/* Execute a single command - simple approach */
static void	exec_cmd_simple(t_cmds *cmd, t_child_context *ctx, t_env **env)
{
	setup_child_exec_env(cmd, ctx->i, ctx->n, ctx->pipes);
	do_exec_cmd(cmd, env);
}

/* Fork and execute all commands in pipeline */
static int	exec_pipeline_commands(t_cmds *arr, int count, t_env **env,
		int pipes[][2])
{
	int				i;
	int				pid;
	int				last_pid;
	t_child_context	ctx;

	i = 0;
	last_pid = -1;
	ctx.n = count;
	ctx.pipes = pipes;
	while (i < count)
	{
		pid = fork();
		if (pid == 0)
		{
			ctx.i = i;
			exec_cmd_simple(&arr[i], &ctx, env);
		}
		else if (handle_fork_result(pid, count, pipes, &last_pid))
			return (1);
		i++;
	}
	return (last_pid);
}

/* Execute commands directly from parsed structure */
int	ms_exec_parsed(t_cmds *arr, int count, t_env **env)
{
	int	pipes[255][2];
	int	last_pid;
	int	status;

	if (!arr || count <= 0)
		return (0);
	if (count == 1 && should_run_in_parent(&arr[0]))
		return (exec_single_builtin(&arr[0], env));
	if (create_pipes(count, pipes) == -1)
		return (1);
	last_pid = exec_pipeline_commands(arr, count, env, pipes);
	close_pipes_all(count - 1, pipes);
	if (last_pid == -1)
		return (1);
	status = wait_children(last_pid, count);
	return (status);
}
