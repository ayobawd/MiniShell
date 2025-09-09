/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_conditions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if command should run in parent process */
bool	should_run_in_parent(t_cmds *cmd)
{
	if (!cmd->cmds || !cmd->cmds[0] || !ms_is_builtin(cmd->cmds[0]))
		return (false);
	if (!ft_strncmp(cmd->cmds[0], "cd", 2) && cmd->cmds[0][2] == '\0')
		return (true);
	if (!ft_strncmp(cmd->cmds[0], "export", 6) && cmd->cmds[0][6] == '\0')
		return (true);
	if (!ft_strncmp(cmd->cmds[0], "unset", 5) && cmd->cmds[0][5] == '\0')
		return (true);
	if (!ft_strncmp(cmd->cmds[0], "exit", 4) && cmd->cmds[0][4] == '\0')
		return (true);
	return (false);
}
