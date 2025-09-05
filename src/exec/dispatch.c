/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ms_status_get(void);
void ms_status_set(int st);

int ms_is_builtin(const char *name)
{
    if (!name)
        return (0);
    return (!ft_strncmp(name, "echo", 5)
        || !ft_strncmp(name, "cd", 3)
        || !ft_strncmp(name, "pwd", 4)
        || !ft_strncmp(name, "export", 7)
        || !ft_strncmp(name, "unset", 6)
        || !ft_strncmp(name, "env", 4)
        || !ft_strncmp(name, "exit", 5));
}

int ms_run_builtin(char **argv, t_env **env, bool in_parent)
{
    if (!ft_strncmp(argv[0], "echo", 5))
        return (bi_echo(argv));
    if (!ft_strncmp(argv[0], "cd", 3))
        return (bi_cd(argv, env));
    if (!ft_strncmp(argv[0], "pwd", 4))
        return (bi_pwd(argv));
    if (!ft_strncmp(argv[0], "export", 7))
        return (bi_export(argv, env));
    if (!ft_strncmp(argv[0], "unset", 6))
        return (bi_unset(argv, env));
    if (!ft_strncmp(argv[0], "env", 4))
        return (bi_env(argv, env));
    if (!ft_strncmp(argv[0], "exit", 5))
        return (bi_exit(argv, env, in_parent));
    return (1);
}

int ms_execute_line(t_cmd *pipeline, t_env **env)
{
    int     status;
    t_cmd   *single;

    if (!pipeline)
        return (0);
    single = pipeline;
    if (!single->next && single->argv && single->argv[0]
        && ms_is_builtin(single->argv[0])
        && (!ft_strncmp(single->argv[0], "cd", 3)
            || !ft_strncmp(single->argv[0], "export", 7)
            || !ft_strncmp(single->argv[0], "unset", 6)
            || !ft_strncmp(single->argv[0], "exit", 5)))
    {
        status = ms_run_builtin(single->argv, env, true);
        ms_status_set(status);
        return (status);
    }
    status = ms_exec_pipeline(pipeline, env);
    ms_status_set(status);
    return (status);
}
