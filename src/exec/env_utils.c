/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>

/* Get value from environment linked list */
char	*ms_env_get(t_env *env, const char *key)
{
	size_t	key_len;
	t_env	*curr;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	curr = env;
	while (curr)
	{
		if (curr->key && ft_strncmp(curr->key, key, key_len) == 0
			&& curr->key[key_len] == '\0')
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

/* Count environment variables */
static int	count_env_vars(t_env *env)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env;
	while (curr)
	{
		if (curr->key && curr->value)
			count++;
		curr = curr->next;
	}
	return (count);
}

/* Convert environment linked list to envp array */
char	**ms_env_to_envp(t_env *env)
{
	char	**envp;
	char	*temp;
	t_env	*curr;
	int		i;
	int		count;

	count = count_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	curr = env;
	while (curr && i < count)
	{
		if (curr->key && curr->value)
		{
			temp = ft_strjoin(curr->key, "=");
			if (temp)
			{
				envp[i] = ft_strjoin(temp, curr->value);
				free(temp);
				if (!envp[i])
					break ;
				i++;
			}
		}
		curr = curr->next;
	}
	envp[i] = NULL;
	return (envp);
}

/* Set environment variable - used by export builtin */
void	ms_export(t_env **env, const char *key, const char *value)
{
	t_env	*curr;
	t_env	*new_env;

	if (!env || !key)
		return ;
	curr = *env;
	while (curr)
	{
		if (curr->key && ft_strncmp(curr->key, key, ft_strlen(key)) == 0
			&& curr->key[ft_strlen(key)] == '\0')
		{
			free(curr->value);
			curr->value = ft_strdup(value ? value : "");
			return ;
		}
		curr = curr->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value ? value : "");
	new_env->next = *env;
	*env = new_env;
}

/* Free environment linked list */
void	ms_env_free(t_env *env)
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

/* Unset environment variable - used by unset builtin */
void	ms_unset(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	
	/* Handle first node */
	if ((*env)->key && ft_strncmp((*env)->key, key, ft_strlen(key)) == 0
		&& (*env)->key[ft_strlen(key)] == '\0')
	{
		curr = *env;
		*env = (*env)->next;
		free(curr->key);
		free(curr->value);
		free(curr);
		return ;
	}
	
	/* Handle remaining nodes */
	prev = *env;
	curr = (*env)->next;
	while (curr)
	{
		if (curr->key && ft_strncmp(curr->key, key, ft_strlen(key)) == 0
			&& curr->key[ft_strlen(key)] == '\0')
		{
			prev->next = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

/* Check if key is valid for environment variable */
int	ms_is_valid_key(const char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	
	/* First character must be letter or underscore */
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	
	/* Rest can be letters, digits, or underscores */
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}