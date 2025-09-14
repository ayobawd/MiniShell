/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>

/* Set redirection type based on flag */
static void	set_redir_type(t_redir *r, int flag)
{
	if (flag == IN_FILE)
		r->type = R_IN;
	else if (flag == OUT_FILE)
		r->type = R_OUT_TRUNC;
	else if (flag == APPEND)
		r->type = R_OUT_APPEND;
	else
		r->type = R_HEREDOC;
}

/* Create a single redirection structure */
static t_redir	*create_single_redir(t_cmds *cmd, int i)
{
	t_redir	*r;

	r = (t_redir *)malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->next = NULL;
	r->fd = -1;
	r->quoted_delim = false;
	set_redir_type(r, cmd->outs[i].flag);
	r->target = ft_strdup(cmd->outs[i].file_name);
	return (r);
}

/* Convert t_redirect to t_redir for a single command */
t_redir	*convert_redirections(t_cmds *cmd)
{
	t_redir	*head;
	t_redir	**pp;
	t_redir	*r;
	int		i;

	head = NULL;
	pp = &head;
	i = 0;
	while (i < cmd->red_len)
	{
		r = create_single_redir(cmd, i);
		if (!r)
			return (head);
		*pp = r;
		pp = &r->next;
		i++;
	}
	return (head);
}

/* Free redirection list */
void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->target);
		free(redirs);
		redirs = next;
	}
}
