/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirects(t_cmds *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->red_len)
	{
		if (cmd->outs && cmd->outs[i].file_name)
			free(cmd->outs[i].file_name);
		i++;
	}
	free(cmd->outs);
}

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

void	free_all(t_shell *pipe, t_cmds *cmd)
{
	int		i;

	if (pipe && pipe->cmds)
	{
		i = 0;
		while (i < pipe->cmd_len)
			free(pipe->cmds[i++]);
		free(pipe->cmds);
		pipe->cmds = NULL;
	}
	if (cmd)
	{
		i = 0;
		while (i < pipe->cmd_len)
		{
			free_strings(cmd[i].cmds);
			if (cmd[i].outs)
				free_redirects(&cmd[i]);
			i++;
		}
		free(cmd);
	}
}

static void	free_env_content(t_list *env_node)
{
	if (env_node->content)
		free(env_node->content);
	free(env_node);
}

void	free_environment(t_shell *pipe)
{
	t_list	*tmp;

	while (pipe && pipe->environment)
	{
		tmp = pipe->environment->next;
		free_env_content(pipe->environment);
		pipe->environment = tmp;
	}
}
