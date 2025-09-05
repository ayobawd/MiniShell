/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adapter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* forward from exec/dispatch.c */
int ms_is_builtin(const char *name);

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

static t_redir *redir_from_parse(t_cmds *pc)
{
    t_redir *head = NULL;
    t_redir **pp = &head;
    int      i;

    i = 0;
    while (i < pc->red_len)
    {
        t_redir *r = (t_redir *)malloc(sizeof(t_redir));
        if (!r)
            return (head);
        r->next = NULL;
        r->fd = -1;
        r->quoted_delim = false;
        if (pc->outs[i].flag == IN_FILE)
            r->type = R_IN;
        else if (pc->outs[i].flag == OUT_FILE)
            r->type = R_OUT_TRUNC;
        else if (pc->outs[i].flag == APPEND)
            r->type = R_OUT_APPEND;
        else /* HERE_DOC */
            r->type = R_HEREDOC;
        r->target = pc->outs[i].file_name; /* take ownership */
        *pp = r; pp = &r->next; i++;
    }
    return (head);
}

static void free_parse_shell_cmds(t_shell *ps)
{
    int i = 0;
    if (!ps->cmds)
        return ;
    while (ps->cmds[i])
    {
        free(ps->cmds[i]);
        i++;
    }
    free(ps->cmds);
}

t_cmd *parse_to_pipeline(char *line, t_env *env)
{
    t_shell ps;
    t_cmds  *arr;
    t_cmd   *head;
    t_cmd   **pp;
    int     j;

    if (!line || spaces(line))
        return (NULL);
    exit_code = ms_status_get();
    ft_bzero(&ps, sizeof(ps));
    char **_envp_for_parse = set_parse_env_from_env(&ps, env);

    /* redirection syntax check first */
    if (!redirections_parse(line))
    {
        write(2, "syntax error, unexpected redirection token\n", 44);
        ms_status_set(258);
        free_env_list_only(&ps.environment);
        return (NULL);
    }
    /* split by pipes and normalize spacing */
    if (!handle_pipes(&ps, line, NULL))
    {
        write(2, "Error\n", 6);
        ms_status_set(258);
        free_env_list_only(&ps.environment);
        return (NULL);
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

    head = NULL; pp = &head;
    j = 0;
    while (j < ps.cmd_len)
    {
        t_cmd *node = (t_cmd *)malloc(sizeof(t_cmd));
        if (!node)
            break ;
        node->next = NULL;
        node->redirs = redir_from_parse(&arr[j]);
        node->argv = arr[j].cmds; /* take ownership */
        node->is_builtin = (node->argv && node->argv[0]) ? ms_is_builtin(node->argv[0]) : false;
        /* free just the array of outs, we moved the strings to redirs */
        if (arr[j].red_len > 0)
            free(arr[j].outs);
        *pp = node; pp = &node->next;
        j++;
    }
    free(arr);
    free_parse_shell_cmds(&ps);
    // free env list nodes and backing strings
    if (_envp_for_parse)
    {
        size_t k = 0;
        while (_envp_for_parse[k])
            free(_envp_for_parse[k++]);
        free(_envp_for_parse);
    }
    free_env_list_only(&ps.environment);
    return (head);
}
