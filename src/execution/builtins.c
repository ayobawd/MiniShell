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

int	builtin_echo(t_cmds *cmd)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (cmd->cmds[1] && ft_strncmp(cmd->cmds[1], "-n", 3) == 0)
	{
		newline = 0;
		i = 2;
	}
	while (cmd->cmds[i])
	{
		printf("%s", cmd->cmds[i]);
		if (cmd->cmds[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
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

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	builtin_env(t_shell *shell)
{
	t_list	*current;

	current = shell->environment;
	while (current)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
	return (0);
}

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