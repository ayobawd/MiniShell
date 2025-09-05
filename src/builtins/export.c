/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int ms_is_valid_key(const char *s);

static void print_err(const char *prefix, const char *name, const char *msg)
{
    write(2, prefix, ft_strlen(prefix));
    write(2, name, ft_strlen(name));
    write(2, msg, ft_strlen(msg));
}

static void print_declare(t_env *env)
{
    t_env *cur;
    int     n;
    int     i;
    char  **rows;
    char   *line;

    n = 0; cur = env;
    while (cur) { n++; cur = cur->next; }
    rows = (char **)malloc(sizeof(char *) * n);
    if (!rows) return ;
    i = 0; cur = env;
    while (cur)
    {
        if (cur->val)
        {
            char *prefix = ft_strjoin("declare -x ", cur->key);
            char *eq = ft_strjoin(prefix, "=\"");
            char *val = ft_strjoin(eq, cur->val);
            line = ft_strjoin(val, "\"\n");
            free(prefix); free(eq); free(val);
        }
        else
        {
            char *prefix = ft_strjoin("declare -x ", cur->key);
            line = ft_strjoin(prefix, "\n");
            free(prefix);
        }
        rows[i++] = line;
        cur = cur->next;
    }
    // simple bubble sort by lexicographic order
    int swapped = 1; char *tmp;
    while (swapped)
    {
        swapped = 0;
        for (i = 1; i < n; i++)
        {
            if (ft_strncmp(rows[i - 1], rows[i], ft_strlen(rows[i - 1]) + 1) > 0)
            { tmp = rows[i - 1]; rows[i - 1] = rows[i]; rows[i] = tmp; swapped = 1; }
        }
    }
    for (i = 0; i < n; i++)
    { write(1, rows[i], ft_strlen(rows[i])); free(rows[i]); }
    free(rows);
}

int bi_export(char **argv, t_env **env)
{
    int     i;
    int     ret;
    char    *eq;

    ret = 0;
    i = 1;
    if (!argv[i])
    {
        print_declare(*env);
        return (0);
    }
    while (argv[i])
    {
        eq = ft_strchr(argv[i], '=');
        if (eq)
        {
            *eq = '\0';
            if (!ms_is_valid_key(argv[i]))
            { ret = 1; print_err("export: `", argv[i], "': not a valid identifier\n"); }
            else
                ms_export(env, argv[i], eq + 1);
            *eq = '=';
        }
        else
        {
            if (!ms_is_valid_key(argv[i]))
            { ret = 1; print_err("export: `", argv[i], "': not a valid identifier\n"); }
            else
                ms_export(env, argv[i], NULL);
        }
        i++;
    }
    return (ret);
}
