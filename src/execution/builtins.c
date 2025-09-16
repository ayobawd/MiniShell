/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
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
		return (builtin_exit(cmd));
	return (0);
}

int	builtin_cd(t_shell *shell, t_cmds *cmd)
{
	char	*path;
	char	*home;

	if (!cmd->cmds[1])
	{
		home = my_getenv("HOME", shell);
		if (!home)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else
	{
		path = cmd->cmds[1];
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_exit(t_cmds *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->cmds[1])
	{
		exit_code = ft_atoi(cmd->cmds[1]);
	}
	printf("exit\n");
	exit(exit_code);
	return (exit_code);
}