/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_cmd_segment(t_shell *pipe, t_cmds *cmds, t_variables *var)
{
	var->start = 0;
	var->quote_char = 0;
	var->h = 0;
	var->x = 0;
	cmds[var->j].red_len = num_of_redirects(pipe->cmds[var->j]);
	if (cmds[var->j].red_len)
		cmds[var->j].outs = malloc(sizeof(t_redirect) * cmds[var->j].red_len);
	utils_saving(pipe, cmds, var);
	cmds[var->j].cmds = quote_aware_split(pipe->cmds[var->j]);
	var->h = 0;
	while (cmds[var->j].cmds[var->h])
		clean_quotes(cmds[var->j].cmds[var->h++]);
	var->h = 0;
	while (cmds[var->j].cmds[var->h])
		var->h++;
}
