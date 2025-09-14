/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by aradwan           #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/* Simple execution function that works with parsing output */
int	simple_execute(t_cmds *cmds, int cmd_count, t_shell *shell)
{
	int		i;
	pid_t	pid;
	int		status;

	(void)shell;
	i = 0;
	while (i < cmd_count)
	{
		if (!cmds[i].cmds || !cmds[i].cmds[0])
		{
			i++;
			continue;
		}
		
		/* Simple builtin check */
		if (!ft_strncmp(cmds[i].cmds[0], "echo", 4) && cmds[i].cmds[0][4] == '\0')
		{
			bi_echo(cmds[i].cmds);
		}
		else if (!ft_strncmp(cmds[i].cmds[0], "pwd", 3) && cmds[i].cmds[0][3] == '\0')
		{
			bi_pwd(cmds[i].cmds);
		}
		else if (!ft_strncmp(cmds[i].cmds[0], "exit", 4) && cmds[i].cmds[0][4] == '\0')
		{
			exit(bi_exit(cmds[i].cmds, NULL, true));
		}
		else
		{
			/* Fork and exec for external commands */
			pid = fork();
			if (pid == 0)
			{
				/* Child process */
				execvp(cmds[i].cmds[0], cmds[i].cmds);
				perror("execvp failed");
				exit(127);
			}
			else if (pid > 0)
			{
				/* Parent process */
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					g_exit_code = WEXITSTATUS(status);
			}
			else
			{
				perror("fork failed");
				return (1);
			}
		}
		i++;
	}
	return (0);
}