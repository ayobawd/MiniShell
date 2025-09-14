/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by aradwan           #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

/* Simple builtin check */
static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 4) == 0 && cmd[4] == '\0')
		return (1);
	return (0);
}

/* Execute a single builtin command */
static int	exec_builtin(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 4) == 0 && argv[0][4] == '\0')
		return (bi_echo(argv));
	return (1);
}

/* Simple external command execution */
static int	exec_external(char **argv, char **env)
{
	pid_t	pid;
	int		status;
	char	*path;

	/* Try to find command in PATH or use absolute path */
	path = NULL;
	if (ft_strchr(argv[0], '/'))
		path = argv[0];
	else
	{
		/* Simple PATH search - just try /bin/ prefix for common commands */
		path = malloc(ft_strlen(argv[0]) + 6);
		if (path)
		{
			ft_strlcpy(path, "/bin/", 6);
			ft_strlcat(path, argv[0], ft_strlen(argv[0]) + 6);
		}
	}
	
	pid = fork();
	if (pid == 0)
	{
		/* Child process */
		execve(path, argv, env);
		perror("execve failed");
		exit(127);
	}
	else if (pid > 0)
	{
		/* Parent process */
		waitpid(pid, &status, 0);
		if (path != argv[0])
			free(path);
		return (WEXITSTATUS(status));
	}
	else
	{
		perror("fork failed");
		if (path != argv[0])
			free(path);
		return (1);
	}
}

/* Simple command execution for t_cmds structure */
int	simple_execute(t_cmds *cmds, int cmd_count, t_shell *shell)
{
	int		i;
	int		status;
	char	**env;

	(void)shell;
	status = 0;
	
	if (!cmds)
		return (1);
	
	/* Create a basic environment array */
	env = malloc(sizeof(char *) * 2);
	if (!env)
		return (1);
	env[0] = "PATH=/bin:/usr/bin";
	env[1] = NULL;

	i = 0;
	while (i < cmd_count)
	{
		if (!cmds[i].cmds || !cmds[i].cmds[0])
		{
			i++;
			continue;
		}
		
		if (is_builtin(cmds[i].cmds[0]))
			status = exec_builtin(cmds[i].cmds);
		else
			status = exec_external(cmds[i].cmds, env);
		
		i++;
	}
	
	free(env);
	return (status);
}