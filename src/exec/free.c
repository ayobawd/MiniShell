/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_redirs(t_redir *r)
{
	t_redir	*n;

	while (r)
	{
		n = r->next;
		if (r->fd != -1)
			close(r->fd);
		free(r->target);
		free(r);
		r = n;
	}
}

void	ms_cmd_free(t_cmd *p)
{
	t_cmd	*n;
	int		i;

	while (p)
	{
		n = p->next;
		if (p->argv)
		{
			i = 0;
			while (p->argv[i])
				free(p->argv[i++]);
			free(p->argv);
		}
		free_redirs(p->redirs);
		free(p);
		p = n;
	}
}
