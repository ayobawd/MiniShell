/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "minishell.h"

static size_t	ms_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

static void	copy_string(char *dest, const char *src, size_t *offset)
{
	size_t	i;

	i = 0;
	while (src && src[i])
	{
		dest[*offset + i] = src[i];
		i++;
	}
	*offset += i;
}

static char	*ms_strjoin3(const char *a, const char *b, const char *c)
{
	size_t	la;
	size_t	lb;
	size_t	lc;
	char	*s;
	size_t	offset;

	la = ms_strlen(a);
	lb = ms_strlen(b);
	lc = ms_strlen(c);
	s = (char *)malloc(la + lb + lc + 1);
	if (!s)
		return (NULL);
	offset = 0;
	copy_string(s, a, &offset);
	copy_string(s, b, &offset);
	copy_string(s, c, &offset);
	s[la + lb + lc] = '\0';
	return (s);
}

char	*ms_prompt(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*dir;
	char	*prompt;

	(void)env;
	if (!getcwd(cwd, sizeof(cwd)))
		dir = (char *)"minishell";
	else
		dir = cwd;
	prompt = ms_strjoin3(dir, (const char *)" $ ", (const char *)"");
	if (!prompt)
		prompt = ft_strdup("minishell $ ");
	return (prompt);
}
