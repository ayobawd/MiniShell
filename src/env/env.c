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

static t_env *new_kv(const char *k, const char *v)
{
    t_env *n = (t_env *)malloc(sizeof(t_env));
    if (!n)
        return (NULL);
    n->key = k ? ft_strdup(k) : NULL;
    n->val = v ? ft_strdup(v) : NULL;
    n->next = NULL;
    if ((k && !n->key) || (v && !n->val))
    {   free(n->key); free(n->val); free(n); return (NULL); }
    return (n);
}

static void env_add_back(t_env **lst, t_env *n)
{
    t_env *cur;
    if (!*lst) { *lst = n; return ; }
    cur = *lst;
    while (cur->next) cur = cur->next;
    cur->next = n;
}

t_env *ms_env_from_environ(char **environ)
{
    t_env *env = NULL;
    size_t i = 0; char *eq;
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

char *ms_env_get(t_env *env, const char *key)
{
    while (env)
    {
        if (env->key && key && ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
            return (env->val);
        env = env->next;
    }
    return (NULL);
}

int ms_export(t_env **env, const char *key, const char *val)
{
    t_env *cur;
    if (!key || !*key)
        return (1);
    cur = *env;
    while (cur)
    {
        if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
        {
            free(cur->val);
            cur->val = val ? ft_strdup(val) : NULL;
            return (0);
        }
        cur = cur->next;
    }
    env_add_back(env, new_kv(key, val));
    return (0);
}

int ms_unset(t_env **env, const char *key)
{
    t_env *cur; t_env *prev = NULL;
    if (!env || !*env || !key)
        return (0);
    cur = *env;
    while (cur)
    {
        if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
        {
            if (prev) prev->next = cur->next; else *env = cur->next;
            free(cur->key); free(cur->val); free(cur);
            return (0);
        }
        prev = cur; cur = cur->next;
    }
    return (0);
}

void ms_env_free(t_env *env)
{
    t_env *n;
    while (env)
    {
        n = env->next;
        free(env->key);
        free(env->val);
        free(env);
        env = n;
    }
}

char **ms_env_to_envp(t_env *env)
{
    size_t count = 0; t_env *cur = env;
    char **envp; size_t i = 0; size_t klen, vlen; char *s;
    while (cur) { count++; cur = cur->next; }
    envp = (char **)malloc(sizeof(char *) * (count + 1));
    if (!envp) return (NULL);
    cur = env;
    while (cur)
    {
        klen = ft_strlen(cur->key);
        vlen = cur->val ? ft_strlen(cur->val) : 0;
        s = (char *)malloc(klen + 1 + vlen + 1);
        if (!s) { while (i--) free(envp[i]); free(envp); return (NULL); }
        ft_memcpy(s, cur->key, klen); s[klen] = '=';
        if (cur->val)
            ft_memcpy(s + klen + 1, cur->val, vlen);
        s[klen + 1 + vlen] = '\0';
        envp[i++] = s;
        cur = cur->next;
    }
    envp[i] = NULL;
    return (envp);
}
