/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
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

int	open_pipes(int n, int pipes[][2])
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

void	close_pipes_all(int n, int pipes[][2])
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

void	setup_child_pipes(int i, int n, int pipes[][2], int fds[2])
{
	if (i > 0)
		fds[0] = pipes[i - 1][0];
	else
		fds[0] = -1;
	if (i < n - 1)
		fds[1] = pipes[i][1];
	else
		fds[1] = -1;
	if (i > 0)
		close(pipes[i - 1][1]);
	if (i < n - 1)
		close(pipes[i][0]);
}

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

int	wait_children(int last_pid, int count)
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

/* Cleanup pipes after execution */
void	cleanup_pipes(int n, int (*pipes)[2])
{
	if (pipes)
	{
		if (n > 1)
		{
			close(pipes[n - 2][0]);
			close(pipes[n - 2][1]);
		}
		free(pipes);
	}
}

/* Handle parent process after fork */
void	handle_parent_process(int status, int i, int (*pipes)[2],
		int *last_pid)
{
	*last_pid = status;
	if (i > 0)
	{
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
	}
}