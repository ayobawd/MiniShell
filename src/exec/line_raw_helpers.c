/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_raw_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list_only(t_list **lst)
{
	t_list	*n;

	while (lst && *lst)
	{
		n = (*lst)->next;
		free(*lst);
		*lst = n;
	}
}

void	cleanup_parse_data(t_shell *ps, char **envp_for_parse)
{
	int	i;

	i = 0;
	if (ps->cmds)
	{
		while (ps->cmds[i])
		{
			free(ps->cmds[i]);
			i++;
		}
		free(ps->cmds);
	}
	if (envp_for_parse)
	{
		i = 0;
		while (envp_for_parse[i])
			free(envp_for_parse[i++]);
		free(envp_for_parse);
	}
	free_env_list_only(&ps->environment);
}
