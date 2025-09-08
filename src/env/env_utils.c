/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_env	*new_kv(const char *k, const char *v)
{
	t_env	*n;

	n = (t_env *)malloc(sizeof(t_env));
	if (!n)
		return (NULL);
	if (k)
		n->key = ft_strdup(k);
	else
		n->key = NULL;
	if (v)
		n->val = ft_strdup(v);
	else
		n->val = NULL;
	n->next = NULL;
	if ((k && !n->key) || (v && !n->val))
	{
		free(n->key);
		free(n->val);
		free(n);
		return (NULL);
	}
	return (n);
}

void	env_add_back(t_env **lst, t_env *n)
{
	t_env	*cur;

	if (!*lst)
	{
		*lst = n;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = n;
}

void	ms_env_free(t_env *env)
{
	t_env	*n;

	while (env)
	{
		n = env->next;
		free(env->key);
		free(env->val);
		free(env);
		env = n;
	}
}

char	*create_env_string(t_env *cur)
{
	size_t	klen;
	size_t	vlen;
	char	*s;

	klen = ft_strlen(cur->key);
	if (cur->val)
		vlen = ft_strlen(cur->val);
	else
		vlen = 0;
	s = (char *)malloc(klen + 1 + vlen + 1);
	if (!s)
		return (NULL);
	ft_memcpy(s, cur->key, klen);
	s[klen] = '=';
	if (cur->val)
		ft_memcpy(s + klen + 1, cur->val, vlen);
	s[klen + 1 + vlen] = '\0';
	return (s);
}

void	free_envp_on_error(char **envp, size_t i)
{
	while (i--)
		free(envp[i]);
	free(envp);
}
