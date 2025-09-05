/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int ms_run_heredoc(const char *delim, bool quoted, int *out_fd);

static int open_out(t_redir *r)
{
    int flags;
    if (r->type == R_OUT_TRUNC)
        flags = O_CREAT | O_TRUNC | O_WRONLY;
    else
        flags = O_CREAT | O_APPEND | O_WRONLY;
    return (open(r->target, flags, 0644));
}

int ms_apply_redirs(t_redir *rlist, int fds[2])
{
    int fd;
    t_redir *r = rlist;

    if (fds[0] != -1)
    { if (dup2(fds[0], 0) < 0) return (-1); }
    if (fds[1] != -1)
    { if (dup2(fds[1], 1) < 0) return (-1); }
    while (r)
    {
        if (r->type == R_IN)
        {
            fd = open(r->target, O_RDONLY);
            if (fd < 0 || dup2(fd, 0) < 0) return (-1);
            close(fd);
        }
        else if (r->type == R_OUT_TRUNC || r->type == R_OUT_APPEND)
        {
            fd = open_out(r);
            if (fd < 0 || dup2(fd, 1) < 0) return (-1);
            close(fd);
        }
        else if (r->type == R_HEREDOC)
        {
            if (r->fd != -1)
                fd = r->fd;
            else if (ms_run_heredoc(r->target, r->quoted_delim, &fd) != 0)
                return (-1);
            if (dup2(fd, 0) < 0) { close(fd); return (-1); }
            close(fd);
        }
        r = r->next;
    }
    return (0);
}
