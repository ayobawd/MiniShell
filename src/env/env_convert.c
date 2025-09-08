/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static size_t	count_env_nodes(t_env *env)
{
	size_t	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static int	fill_envp_array(char **envp, t_env *env)
{
	size_t	i;

	i = 0;
	while (env)
	{
		envp[i] = create_env_string(env);
		if (!envp[i])
		{
			free_envp_on_error(envp, i);
			return (1);
		}
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (0);
}

t_env	*ms_env_from_environ(char **environ)
{
	t_env	*env;
	size_t	i;
	char	*eq;

	env = NULL;
	i = 0;
	while (environ && environ[i])
	{
		eq = ft_strchr(environ[i], '=');
		if (eq)
		{
			*eq = '\0';
			env_add_back(&env, new_kv(environ[i], eq + 1));
			*eq = '=';
		}
		i++;
	}
	return (env);
}

char	**ms_env_to_envp(t_env *env)
{
	size_t	count;
	char	**envp;

	count = count_env_nodes(env);
	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (fill_envp_array(envp, env))
		return (NULL);
	return (envp);
}
