/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by marvin            #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_env(t_shell *s, char **env)
{
	int	i;

	i = 0;
	s->environment = NULL;
	while (env[i])
	{
		ft_lstadd_back(&s->environment, ft_lstnew(env[i]));
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
		if (strncmp(key, name, name_len) == 0 && key[name_len] == '=')
		{
			value = key + name_len + 1;
			break ;
		}
		env = env->next;
	}
	return (value);
}
