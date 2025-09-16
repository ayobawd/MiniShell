/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_commands(t_shell *shell, t_cmds *cmds)
{
	int	status;

	if (!shell || !cmds || shell->cmd_len == 0)
		return (0);
	status = execute_pipeline(shell, cmds, shell->cmd_len);
	return (status);
}

int	execute_single_command(t_shell *shell, t_cmds *cmd)
{
	if (!cmd || !cmd->cmds || !cmd->cmds[0])
		return (0);
	if (is_builtin(cmd->cmds[0]))
		return (execute_builtin(shell, cmd));
	return (execute_external_command(shell, cmd));
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	fork_and_execute(char *cmd_path, char **cmd_args, char **env_array)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, cmd_args, env_array) == -1)
		{
			perror("minishell");
			exit(126);
		}
	}
	else if (pid > 0)
		return (wait_for_child(pid));
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}

int	execute_external_command(t_shell *shell, t_cmds *cmd)
{
	char	*cmd_path;
	char	**env_array;
	int		status;

	cmd_path = find_command_path(cmd->cmds[0], shell);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", cmd->cmds[0]);
		return (127);
	}
	env_array = create_env_array(shell);
	status = fork_and_execute(cmd_path, cmd->cmds, env_array);
	free(cmd_path);
	free_strings(env_array);
	return (status);
}
