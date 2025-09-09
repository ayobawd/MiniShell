/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

/* Forward declarations */
void	ms_signals_child_default(void);
int		ms_run_builtin(char **argv, t_env **env, bool in_parent);
char	*ms_resolve_path(const char *cmd, t_env *env);
char	**ms_env_to_envp(t_env *env);
int		ms_is_builtin(const char *name);
void	close_pipes_all(int n, int pipes[][2]);
t_redir	*convert_redirections(t_cmds *cmd);
void	free_redirs(t_redir *redirs);

/* Setup pipes for child process */
void	setup_child_fds(int i, int count, int pipes[][2], int fds[2])
{
	if (i > 0)
		fds[0] = pipes[i - 1][0];
	if (i < count - 1)
		fds[1] = pipes[i][1];
	if (i > 0)
		close(pipes[i - 1][1]);
	if (i < count - 1)
		close(pipes[i][0]);
}

/* Handle command execution errors */
void	handle_exec_error(char *cmd_name)
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

/* Execute command with path resolution */
void	exec_with_path(t_cmds *cmd, char **envp, char *path)
{
	if (path)
	{
		execve(path, cmd->cmds, envp);
		free(path);
	}
	else if (ft_strchr(cmd->cmds[0], '/'))
		execve(cmd->cmds[0], cmd->cmds, envp);
	handle_exec_error(cmd->cmds[0]);
}

/* Setup child execution environment */
void	setup_child_exec_env(t_cmds *cmd, int i, int count, int pipes[][2])
{
	int		fds[2];
	t_redir	*redirs;

	fds[0] = -1;
	fds[1] = -1;
	ms_signals_child_default();
	setup_child_fds(i, count, pipes, fds);
	redirs = convert_redirections(cmd);
	if (ms_apply_redirs(redirs, fds) != 0)
	{
		free_redirs(redirs);
		exit(1);
	}
	free_redirs(redirs);
	close_pipes_all(count - 1, pipes);
}

/* Execute command after environment setup */
void	do_exec_cmd(t_cmds *cmd, t_env **env)
{
	char	*path;
	char	**envp;

	if (!cmd->cmds || !cmd->cmds[0] || !cmd->cmds[0][0])
		exit(0);
	if (ms_is_builtin(cmd->cmds[0]))
		exit(ms_run_builtin(cmd->cmds, env, false));
	path = ms_resolve_path(cmd->cmds[0], *env);
	envp = ms_env_to_envp(*env);
	exec_with_path(cmd, envp, path);
}
