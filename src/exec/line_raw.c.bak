/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_raw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Helper functions from parsing */
extern int	g_exit_code;

static void free_env_list_only(t_list **lst)
{
    t_list *n;
    while (lst && *lst)
    {
        n = (*lst)->next;
        free(*lst);
        *lst = n;
    }
}

static char **set_parse_env_from_env(t_shell *ps, t_env *env)
{
    char    **envp;

    envp = ms_env_to_envp(env);
    if (!envp)
        return (NULL);
    copy_env(ps, envp);
    return (envp);
}

static void cleanup_parse_data(t_shell *ps, char **envp_for_parse)
{
    int i = 0;
    
    if (ps->cmds)
    {
        while (ps->cmds[i])
        {
            free(ps->cmds[i]);
            i++;
        }
        free(ps->cmds);
    }
    
    if (envp_for_parse)
    {
        i = 0;
        while (envp_for_parse[i])
            free(envp_for_parse[i++]);
        free(envp_for_parse);
    }
    
    free_env_list_only(&ps->environment);
}

int ms_exec_line_raw(char *line, t_env **env)
{
    t_shell ps;
    t_cmds  *arr;
    int     status;
    int     j;
    char    **envp_for_parse;

    if (!line || spaces(line))
        return (0);
    
    g_exit_code = ms_status_get();
    ft_bzero(&ps, sizeof(ps));
    envp_for_parse = set_parse_env_from_env(&ps, *env);

    /* redirection syntax check first */
    if (!redirections_parse(line))
    {
        write(2, "syntax error, unexpected redirection token\n", 44);
        ms_status_set(258);
        cleanup_parse_data(&ps, envp_for_parse);
        return (258);
    }
    
    /* split by pipes and normalize spacing */
    if (!handle_pipes(&ps, line, NULL))
    {
        write(2, "Error\n", 6);
        ms_status_set(258);
        cleanup_parse_data(&ps, envp_for_parse);
        return (258);
    }
    
    /* expand variables per segment using parser's logic */
    j = 0;
    while (j < ps.cmd_len)
    {
        dollar_expansion(&ps.cmds[j], &ps);
        j++;
    }
    
    /* build parse command array with redirections and tokens */
    files_saving(&ps, &arr);
    
    /* execute the commands directly from parsed structure */
    status = ms_exec_parsed(arr, ps.cmd_len, env);
    
    /* cleanup */
    cleanup_parse_data(&ps, envp_for_parse);
    free(arr);
    
    ms_status_set(status);
    return (status);
}