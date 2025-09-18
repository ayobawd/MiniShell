/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by ayal-awa         ###   ########.fr       */
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

static int	handle_export_with_equal(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	int		ret;
	char	*eq;

	eq = ft_strchr(arg, '=');
	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		ret = print_ident_err("export", key);
		free(key);
		return (ret);
	}
	value = ft_strdup(eq + 1);
	if (!value)
	{
		free(key);
		return (1);
	}
	set_env_var(shell, key, value);
	free(key);
	free(value);
	return (0);
}

static int	handle_export_no_equal(char *arg)
{
	if (!is_valid_identifier(arg))
		return (print_ident_err("export", arg));
	return (0);
}

static int	process_export_arg(t_shell *shell, char *arg)
{
	if (ft_strchr(arg, '='))
		return (handle_export_with_equal(shell, arg));
	return (handle_export_no_equal(arg));
}

int	builtin_export(t_shell *shell, t_cmds *cmd)
{
	int	i;
	int	status;
	int	arg_status;

	if (!cmd->cmds[1])
	{
		return (print_export_env(shell));
	}
	status = 0;
	i = 1;
	while (cmd->cmds[i])
	{
		arg_status = process_export_arg(shell, cmd->cmds[i]);
		if (arg_status != 0)
			status = 1;
		i++;
	}
	return (status);
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
