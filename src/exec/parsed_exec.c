/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

/* Forward declarations */
void ms_signals_child_default(void);
int  ms_run_builtin(char **argv, t_env **env, bool in_parent);
char *ms_resolve_path(const char *cmd, t_env *env);
char **ms_env_to_envp(t_env *env);
int ms_is_builtin(const char *name);

/* Convert t_redirect to t_redir for a single command */
static t_redir *convert_redirections(t_cmds *cmd)
{
	t_redir *head = NULL;
	t_redir **pp = &head;
	int i = 0;

	while (i < cmd->red_len)
	{
	    t_redir *r = (t_redir *)malloc(sizeof(t_redir));
	    if (!r)
	        return (head);
	    r->next = NULL;
	    r->fd = -1;
	    r->quoted_delim = false;
	    if (cmd->outs[i].flag == IN_FILE)
	        r->type = R_IN;
	    else if (cmd->outs[i].flag == OUT_FILE)
	        r->type = R_OUT_TRUNC;
	    else if (cmd->outs[i].flag == APPEND)
	        r->type = R_OUT_APPEND;
	    else /* HERE_DOC */
	        r->type = R_HEREDOC;
	    r->target = ft_strdup(cmd->outs[i].file_name);
	    *pp = r;
	    pp = &r->next;
	    i++;
	}
	return (head);
}

/* Free redirection list */
static void free_redirs(t_redir *redirs)
{
	t_redir *next;
	while (redirs)
	{
	    next = redirs->next;
	    free(redirs->target);
	    free(redirs);
	    redirs = next;
	}
}

/* Create pipes for pipeline */
static int create_pipes(int count, int pipes[][2])
{
	int i = 0;
	while (i < count - 1)
	{
	    if (pipe(pipes[i]) == -1)
	        return (-1);
	    i++;
	}
	return (0);
}

/* Close all pipes */
static void close_pipes_all(int count, int pipes[][2])
{
	int i = 0;
	while (i < count)
	{
	    close(pipes[i][0]);
	    close(pipes[i][1]);
	    i++;
	}
}

/* Execute a single command in the pipeline */
static void exec_single_cmd(t_cmds *cmd, int i, int count, int pipes[][2], t_env **env)
{
	int fds[2] = {-1, -1};
	char *path;
	char **envp;

	ms_signals_child_default();

	/* Set up pipes */
	if (i > 0)
	    fds[0] = pipes[i - 1][0];
	if (i < count - 1)
	    fds[1] = pipes[i][1];

	/* Close other pipe ends */
	if (i > 0) close(pipes[i - 1][1]);
	if (i < count - 1) close(pipes[i][0]);

	/* Convert and apply redirections */
	t_redir *redirs = convert_redirections(cmd);
	if (ms_apply_redirs(redirs, fds) != 0)
	{
	    free_redirs(redirs);
	    exit(1);
	}
	free_redirs(redirs);

	/* Close remaining pipes */
	close_pipes_all(count - 1, pipes);

	/* Execute command */
	if (!cmd->cmds || !cmd->cmds[0] || !cmd->cmds[0][0])
	    exit(0);

	if (ms_is_builtin(cmd->cmds[0]))
	    exit(ms_run_builtin(cmd->cmds, env, false));

	path = ms_resolve_path(cmd->cmds[0], *env);
	envp = ms_env_to_envp(*env);

	if (path)
	{
	    execve(path, cmd->cmds, envp);
	    free(path);
	}
	else if (ft_strchr(cmd->cmds[0], '/'))
	    execve(cmd->cmds[0], cmd->cmds, envp);

	/* if execve returns, it's an error */
	if (errno == ENOENT)
	{
	    write(2, "minishell: ", 11);
	    write(2, cmd->cmds[0], ft_strlen(cmd->cmds[0]));
	    write(2, ": command not found\n", 20);
	    exit(127);
	}
	if (errno == EACCES)
	{
	    write(2, "minishell: ", 11);
	    write(2, cmd->cmds[0], ft_strlen(cmd->cmds[0]));
	    write(2, ": Permission denied\n", 19);
	    exit(126);
	}
	exit(126);
}

/* Wait for all children and return last exit status */
static int wait_children(int last_pid, int count)
{
	int status;
	int ret = 0;
	int pid;

	while (count-- > 0)
	{
	    pid = wait(&status);
	    if (pid == last_pid)
	    {
	        if (WIFEXITED(status))
	            ret = WEXITSTATUS(status);
	        else if (WIFSIGNALED(status))
	            ret = 128 + WTERMSIG(status);
	    }
	}
	return (ret);
}

/* Execute commands directly from parsed structure */
int ms_exec_parsed(t_cmds *arr, int count, t_env **env)
{
	int pipes[255][2];  /* Arbitrary limit like in original */
	int i = 0;
	int pid;
	int last_pid = -1;
	int status;

	if (!arr || count <= 0)
	    return (0);

	/* Special case: single builtin that needs to run in parent */
	if (count == 1 && arr[0].cmds && arr[0].cmds[0]
	    && ms_is_builtin(arr[0].cmds[0])
	    && ((!ft_strncmp(arr[0].cmds[0], "cd", 2) && arr[0].cmds[0][2] == '\0')
	        || (!ft_strncmp(arr[0].cmds[0], "export", 6) && arr[0].cmds[0][6] == '\0')
	        || (!ft_strncmp(arr[0].cmds[0], "unset", 5) && arr[0].cmds[0][5] == '\0')
	        || (!ft_strncmp(arr[0].cmds[0], "exit", 4) && arr[0].cmds[0][4] == '\0')))
	{
	    t_redir *redirs = convert_redirections(&arr[0]);
	    int fds[2] = {-1, -1};
	    if (ms_apply_redirs(redirs, fds) != 0)
	    {
	        free_redirs(redirs);
	        return (1);
	    }
	    status = ms_run_builtin(arr[0].cmds, env, true);
	    free_redirs(redirs);
	    return (status);
	}

	/* Create pipes for pipeline */
	if (create_pipes(count, pipes) == -1)
	    return (1);

	/* Fork and execute each command */
	while (i < count)
	{
	    pid = fork();
	    if (pid == 0)
	        exec_single_cmd(&arr[i], i, count, pipes, env);
	    else if (pid > 0)
	        last_pid = pid;
	    else
	    {
	        close_pipes_all(count - 1, pipes);
	        return (1);
	    }
	    i++;
	}

	/* Close all pipes in parent */
	close_pipes_all(count - 1, pipes);

	/* Wait for all children */
	status = wait_children(last_pid, count);
	return (status);
}