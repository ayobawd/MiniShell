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

static int	open_pipes(int n, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

static void	close_pipes_all(int n, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static void	setup_child_pipes(int i, int n, int pipes[][2], int fds[2])
{
	fds[0] = (i > 0) ? pipes[i - 1][0] : -1;
	fds[1] = (i < n - 1) ? pipes[i][1] : -1;
	if (i > 0)
		close(pipes[i - 1][1]);
	if (i < n - 1)
		close(pipes[i][0]);
}

static void	handle_exec_error(char *cmd_name)
{
	if (errno == ENOENT)
	{
		write(2, "minishell: ", 11);
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	if (errno == EACCES)
	{
		write(2, "minishell: ", 11);
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": Permission denied\n", 19);
		exit(126);
	}
	exit(126);
}

static void	child_exec(t_cmd *cmd, t_env **env, int i, int n, int pipes[][2])
{
	int		fds[2];
	char	**envp;
	char	*path;

	ms_signals_child_default();
	setup_child_pipes(i, n, pipes, fds);
	if (ms_apply_redirs(cmd->redirs, fds) != 0)
		exit(1);
	close_pipes_all(n - 1, pipes);
	if (cmd->is_builtin)
		exit(ms_run_builtin(cmd->argv, env, false));
	path = ms_resolve_path(cmd->argv[0], *env);
	envp = ms_env_to_envp(*env);
	if (!cmd->argv[0] || !cmd->argv[0][0])
		exit(0);
	if (path)
	{
		execve(path, cmd->argv, envp);
		free(path);
	}
	else if (ft_strchr(cmd->argv[0], '/'))
		execve(cmd->argv[0], cmd->argv, envp);
	handle_exec_error(cmd->argv[0]);
}

static int	wait_children(int last_pid, int count)
{
	int	status;
	int	ret;
	int	pid;

	ret = 0;
	while (count-- > 0)
	{
		pid = wait(&status);
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				ret = 128 + WTERMSIG(status);
		}
	}
	return (ret);
}

int ms_exec_pipeline(t_cmd *first, t_env **env)
{
	int     n = 0; t_cmd *cur = first; int i;
	int     last_pid = -1; int status;
	int     (*pipes)[2];

	while (cur) { n++; cur = cur->next; }
	pipes = NULL;
	if (n > 1)
	{
	    pipes = (int (*)[2])malloc(sizeof(int[2]) * (n - 1));
	    if (!pipes) return (1);
	    if (open_pipes(n - 1, pipes) != 0) { free(pipes); return (1); }
	}
	i = 0; cur = first;
	while (cur)
	{
	    int pid = fork();
	    if (pid == 0)
	        child_exec(cur, env, i, n, pipes);
	    else if (pid < 0)
	    { status = 1; break ; }
	    else
	    {
	        last_pid = pid;
	        if (i > 0) { close(pipes[i - 1][0]); close(pipes[i - 1][1]); }
	    }
	    i++; cur = cur->next;
	}
	if (pipes) { if (n > 1) { close(pipes[n - 2][0]); close(pipes[n - 2][1]); } free(pipes); }
	if (last_pid == -1)
	    return (1);
	status = wait_children(last_pid, n);
	return (status);
}
