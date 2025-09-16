/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_commands(t_shell *shell, t_cmds *cmds)
{
	int	i;
	int	status;

	if (!shell || !cmds || shell->cmd_len == 0)
		return (0);
	
	status = 0;
	i = 0;
	while (i < shell->cmd_len)
	{
		if (cmds[i].cmds && cmds[i].cmds[0])
		{
			status = execute_single_command(shell, &cmds[i]);
		}
		i++;
	}
	return (status);
}

int	execute_single_command(t_shell *shell, t_cmds *cmd)
{
	if (!cmd || !cmd->cmds || !cmd->cmds[0])
		return (0);
		
	// Check if it's a builtin command
	if (is_builtin(cmd->cmds[0]))
		return (execute_builtin(shell, cmd));
	
	// Execute external command
	return (execute_external_command(shell, cmd));
}

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

char	*find_command_path(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	// Check if command is already a full path or relative path
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	// Get PATH environment variable
	path_env = my_getenv("PATH", shell);
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

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

int	execute_external_command(t_shell *shell, t_cmds *cmd)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**env_array;

	cmd_path = find_command_path(cmd->cmds[0], shell);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", cmd->cmds[0]);
		return (127);
	}

	env_array = create_env_array(shell);
	
	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (execve(cmd_path, cmd->cmds, env_array) == -1)
		{
			perror("minishell");
			exit(126);
		}
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else
			status = 1;
	}
	else
	{
		perror("fork");
		status = 1;
	}

	free(cmd_path);
	free_strings(env_array);
	return (status);
}