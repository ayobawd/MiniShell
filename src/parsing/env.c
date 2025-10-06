/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:54:58 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:54:58 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	copy_env(t_shell *s, char **env)
{
	int		i;
	char	*dup;
	t_list	*node;

	i = 0;
	s->environment = NULL;
	while (env[i])
	{
		dup = ft_strdup(env[i]);
		if (!dup)
			return ;
		node = ft_lstnew(dup);
		if (!node)
		{
			free(dup);
			return ;
		}
		ft_lstadd_back(&s->environment, node);
		i++;
	}
	s->environment_num = i;
}

char	*my_getenv(const char *name, t_shell *pipe)
{
	char	*value;
	size_t	name_len;
	t_list	*env;
	char	*key;

	value = NULL;
	name_len = ft_strlen(name);
	env = pipe->environment;
	while (env != NULL)
	{
		key = (char *) env->content;
		if (ft_strncmp(key, name, name_len) == 0 && key[name_len] == '=')
		{
			value = key + name_len + 1;
			break ;
		}
		env = env->next;
	}
	return (value);
}
