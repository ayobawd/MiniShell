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

	if (!av)
		return ;
	i = 0;
	while (av[i])
		free(av[i++]);
	free(av);
}

static void	free_redirections(t_redirect *outs, int red_len)
{
	int	i;

	if (!outs)
		return ;
	i = 0;
	while (i < red_len)
	{
		if (outs[i].file_name)
			free(outs[i].file_name);
		i++;
	}
	free(outs);
}

void	free_all(t_shell *pipe, t_cmds *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (i < pipe->cmd_len)
	{
		free_strings(cmd[i].cmds);
		free_redirections(cmd[i].outs, cmd[i].red_len);
		i++;
	}
	free(cmd);
}