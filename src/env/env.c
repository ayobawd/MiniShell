/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

char	*ms_env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (env->key && key && ft_strncmp(env->key, key,
				ft_strlen(key) + 1) == 0)
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

int	ms_export(t_env **env, const char *key, const char *val)
{
	t_env	*cur;

	if (!key || !*key)
		return (1);
	cur = *env;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
		{
			free(cur->val);
			if (val)
				cur->val = ft_strdup(val);
			else
				cur->val = NULL;
			return (0);
		}
		cur = cur->next;
	}
	env_add_back(env, new_kv(key, val));
	return (0);
}

int	ms_unset(t_env **env, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (!env || !*env || !key)
		return (0);
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->val);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
