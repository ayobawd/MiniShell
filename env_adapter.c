/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_adapter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exec integration <exec@integration.fix>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by exec              #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by exec             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0 
			&& env->key[ft_strlen(key)] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	ms_export(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 
			&& current->key[ft_strlen(key)] == '\0')
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (0);
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (1);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = *env;
	*env = new_node;
	return (0);
}

char	**ms_env_to_envp(t_env *env)
{
	t_env	*current;
	char	**envp;
	int		count;
	int		i;
	char	*temp;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	current = env;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		envp[i] = ft_strjoin(temp, current->value);
		free(temp);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

t_env	*shell_to_exec_env(t_shell *shell)
{
	t_list	*current;
	t_env	*exec_env;
	t_env	*new_node;
	char	*env_str;
	char	*equals_pos;

	exec_env = NULL;
	current = shell->environment;
	while (current)
	{
		env_str = (char *)current->content;
		equals_pos = ft_strchr(env_str, '=');
		if (equals_pos)
		{
			new_node = malloc(sizeof(t_env));
			if (!new_node)
				return (exec_env);
			new_node->key = ft_substr(env_str, 0, equals_pos - env_str);
			new_node->value = ft_strdup(equals_pos + 1);
			new_node->next = exec_env;
			exec_env = new_node;
		}
		current = current->next;
	}
	return (exec_env);
}

void	free_exec_env(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
}

void	ms_signals_child_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	ms_is_valid_key(const char *s)
{
	int	i;

	if (!s || !s[0] || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	ms_unset(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 
			&& current->key[ft_strlen(key)] == '\0')
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	handle_exec_error(const char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	exit(127);
}

int	handle_fork_result(int pid, int count, int pipes[][2], int *last_pid)
{
	if (pid == -1)
	{
		close_pipes_all(count - 1, pipes);
		return (1);
	}
	*last_pid = pid;
	return (0);
}

void	close_pipes_all(int n, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}