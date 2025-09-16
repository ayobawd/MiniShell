/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipelines_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
			return (WEXITSTATUS(status));
		return (1);
	}
	else
	{
		perror("fork");
		return (1);
	}
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

int	execute_single_command_with_redirections(t_shell *shell, t_cmds *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (is_builtin(cmd->cmds[0]) && !should_fork_builtin(cmd))
		status = execute_builtin_with_redirect(shell, cmd, saved_stdin,
				saved_stdout);
	else
	{
		status = fork_and_execute_command(shell, cmd);
		restore_fds(saved_stdin, saved_stdout);
	}
	return (status);
}
