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

static size_t  ms_strlen(const char *s)
{
    size_t i = 0;
    while (s && s[i])
        i++;
    return (i);
}

static char *ms_strjoin3(const char *a, const char *b, const char *c)
{
    size_t la = ms_strlen(a), lb = ms_strlen(b), lc = ms_strlen(c);
    char *s = (char *)malloc(la + lb + lc + 1);
    size_t i = 0; size_t j = 0;
    if (!s)
        return (NULL);
    while (i < la) { s[i] = a[i]; i++; }
    while (j < lb) { s[i + j] = b[j]; j++; }
    j = 0;
    while (j < lc) { s[i + lb + j] = c[j]; j++; }
    s[la + lb + lc] = '\0';
    return (s);
}

char    *ms_prompt(t_env *env)
{
    char    cwd[PATH_MAX];
    char    *dir;
    char    *prompt;
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
