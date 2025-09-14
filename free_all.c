/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by aradwan           #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strings(char **av)
{
	int	i;

	i = 0;
	if (!av)
		return ;
	while (av[i])
	{
		free(av[i]);
		i++;
	}
	free(av);
}

void	free_all(t_shell *pipe, t_cmds *cmd)
{
	int	i;
	int	j;

	if (pipe->cmds)
	{
		i = 0;
		while (i < pipe->cmd_len)
		{
			free(pipe->cmds[i]);
			i++;
		}
		free(pipe->cmds);
		pipe->cmds = NULL;
	}
	if (cmd)
	{
		i = 0;
		while (i < pipe->cmd_len)
		{
			if (cmd[i].cmds)
				free_strings(cmd[i].cmds);
			if (cmd[i].outs && cmd[i].red_len > 0)
			{
				j = 0;
				while (j < cmd[i].red_len)
				{
					free(cmd[i].outs[j].file_name);
					j++;
				}
				free(cmd[i].outs);
			}
			i++;
		}
		free(cmd);
	}
}