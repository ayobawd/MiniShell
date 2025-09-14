/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adapter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>

/* Convert t_shell environment to t_env linked list */
t_env	*convert_shell_env(t_shell *shell)
{
	t_env	*head;
	t_env	*new_env;
	t_list	*curr;
	char	*env_str;
	char	*eq_pos;

	head = NULL;
	curr = shell->environment;
	while (curr)
	{
		env_str = (char *)curr->content;
		eq_pos = ft_strchr(env_str, '=');
		if (eq_pos)
		{
			new_env = malloc(sizeof(t_env));
			if (!new_env)
				return (head);
			new_env->key = ft_substr(env_str, 0, eq_pos - env_str);
			new_env->value = ft_strdup(eq_pos + 1);
			new_env->next = head;
			head = new_env;
		}
		curr = curr->next;
	}
	return (head);
}

/* Create a single t_cmd from t_cmds */
static t_cmd	*create_cmd_from_cmds(t_cmds *cmd_data)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	/* Note: we don't copy argv, we just reference it since it will be freed by original system */
	cmd->argv = cmd_data->cmds;
	cmd->redirs = convert_redirections(cmd_data);
	cmd->is_builtin = (cmd_data->cmds && cmd_data->cmds[0] && ms_is_builtin(cmd_data->cmds[0]));
	cmd->next = NULL;
	return (cmd);
}

/* Convert array of t_cmds to linked list of t_cmd */
t_cmd	*convert_cmds_to_pipeline(t_cmds *cmds, int cmd_count)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_cmd	*new_cmd;
	int		i;

	if (!cmds || cmd_count <= 0)
		return (NULL);
	head = create_cmd_from_cmds(&cmds[0]);
	if (!head)
		return (NULL);
	curr = head;
	i = 1;
	while (i < cmd_count)
	{
		new_cmd = create_cmd_from_cmds(&cmds[i]);
		if (!new_cmd)
			return (head); // Return partial pipeline on error
		curr->next = new_cmd;
		curr = new_cmd;
		i++;
	}
	return (head);
}

/* Execute commands using the integrated execution system */
int	execute_integrated(t_cmds *cmds, int cmd_count, t_shell *shell)
{
	t_cmd	*pipeline;
	t_env	*env;
	int		status;

	if (!cmds || cmd_count <= 0)
		return (0);
	
	/* Convert shell environment to exec environment */
	env = convert_shell_env(shell);
	if (!env)
		return (1);
	
	/* Convert commands to pipeline */
	pipeline = convert_cmds_to_pipeline(cmds, cmd_count);
	if (!pipeline)
	{
		ms_env_free(env);
		return (1);
	}
	
	/* Execute the pipeline */
	status = ms_execute_line(pipeline, &env);
	
	/* Update global exit code */
	g_exit_code = status;
	
	/* Cleanup */
	ms_cmd_free_no_argv(pipeline);
	ms_env_free(env);
	
	return (status);
}

/* Free redirection list */
static void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->target);
		free(redirs);
		redirs = next;
	}
}

/* Free t_cmd pipeline but not the argv arrays (owned by parsing) */
void	ms_cmd_free_no_argv(t_cmd *p)
{
	t_cmd	*n;

	while (p)
	{
		n = p->next;
		/* Don't free argv since it's owned by the parsing system */
		free_redirs(p->redirs);
		free(p);
		p = n;
	}
}