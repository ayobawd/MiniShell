/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

static char	*search_in_paths(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (temp)
		{
			full_path = ft_strjoin(temp, cmd);
			free(temp);
			if (full_path && access(full_path, F_OK | X_OK) == 0)
			{
				free_strings(paths);
				return (full_path);
			}
			free(full_path);
		}
		i++;
	}
	free_strings(paths);
	return (NULL);
}

char	*find_command_path(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = my_getenv("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(cmd, paths));
}

void	execute_forked_child(t_shell *shell, t_cmds *cmd,
		int saved_stdin, int saved_stdout)
{
	int	exit_code;

	close(saved_stdin);
	close(saved_stdout);
	if (setup_redirections(cmd) == -1)
	{
		free_all(shell, cmd);
		free_environment(shell);
		exit(1);
	}
	if (is_builtin(cmd->cmds[0]))
		exit_code = execute_builtin(shell, cmd);
	else
		exit_code = execute_external_command(shell, cmd);
	free_all(shell, cmd);
	free_environment(shell);
	exit(exit_code);
}
