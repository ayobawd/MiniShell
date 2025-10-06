/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_builtin(t_shell *shell, t_cmds *cmd)
{
	if (!cmd || !cmd->cmds || !cmd->cmds[0])
		return (0);
	if (ft_strncmp(cmd->cmds[0], "echo", 5) == 0)
		return (builtin_echo(cmd));
	if (ft_strncmp(cmd->cmds[0], "cd", 3) == 0)
		return (builtin_cd(shell, cmd));
	if (ft_strncmp(cmd->cmds[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(cmd->cmds[0], "export", 7) == 0)
		return (builtin_export(shell, cmd));
	if (ft_strncmp(cmd->cmds[0], "unset", 6) == 0)
		return (builtin_unset(shell, cmd));
	if (ft_strncmp(cmd->cmds[0], "env", 4) == 0)
		return (builtin_env(shell));
	if (ft_strncmp(cmd->cmds[0], "exit", 5) == 0)
		return (builtin_exit(shell, cmd));
	return (0);
}

static int	cd_home_path(t_shell *shell, char **out)
{
	char	*home;

	home = my_getenv("HOME", shell);
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	*out = home;
	return (0);
}

int	builtin_cd(t_shell *shell, t_cmds *cmd)
{
	char	*path;

	if (cmd->cmds[1] && cmd->cmds[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	path = cmd->cmds[1];
	if (!path)
	{
		if (cd_home_path(shell, &path) != 0)
			return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
