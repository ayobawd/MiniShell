/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	process_export_arg(t_shell *shell, char *arg)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		key = ft_substr(arg, 0, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
		if (key && value)
		{
			set_env_var(shell, key, value);
		}
		free(key);
		free(value);
	}
	else
	{
		if (!is_valid_identifier(arg))
		{
			printf("export: `%s': not a valid identifier\n", arg);
			return (1);
		}
	}
	return (0);
}

int	builtin_export(t_shell *shell, t_cmds *cmd)
{
	int	i;

	if (!cmd->cmds[1])
	{
		return (print_export_env(shell));
	}
	i = 1;
	while (cmd->cmds[i])
	{
		if (process_export_arg(shell, cmd->cmds[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_unset(t_shell *shell, t_cmds *cmd)
{
	int	i;

	if (!cmd->cmds[1])
		return (0);
	i = 1;
	while (cmd->cmds[i])
	{
		if (is_valid_identifier(cmd->cmds[i]))
		{
			unset_env_var(shell, cmd->cmds[i]);
		}
		else
		{
			printf("unset: `%s': not a valid identifier\n", cmd->cmds[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
