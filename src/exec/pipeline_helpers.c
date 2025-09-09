/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
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

/* Handle fork result for pipeline command */
int	handle_fork_result(int pid, int count, int pipes[][2],
		int *last_pid)
{
	if (pid > 0)
		*last_pid = pid;
	else
	{
		close_pipes_all(count - 1, pipes);
		return (1);
	}
	return (0);
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
