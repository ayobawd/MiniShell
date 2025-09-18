/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:00:00 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/12/22 12:00:00 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	print_ident_err(const char *cmd, const char *arg)
{
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	builtin_unset(t_shell *shell, t_cmds *cmd)
{
	int	i;
	int	status;

	if (!cmd->cmds[1])
		return (0);
	status = 0;
	i = 1;
	while (cmd->cmds[i])
	{
		if (is_valid_identifier(cmd->cmds[i]))
		{
			unset_env_var(shell, cmd->cmds[i]);
		}
		else
		{
			status = print_ident_err("unset", cmd->cmds[i]);
		}
		i++;
	}
	return (status);
}
