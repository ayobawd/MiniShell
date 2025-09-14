/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>

int	ms_status_get(void);

static char	*expand_env_default(const char *s)
{
	(void)s;
	return (NULL);
}

static int	process_heredoc_line(const char *line, const char *delim,
	size_t dlen, bool quoted)
{
	char	*tmp;

	if (!ft_strncmp(line, delim, dlen + 1))
		return (1);
	if (!quoted)
	{
		tmp = expand_env_default(line);
		if (tmp)
		{
			free((char *)line);
			line = tmp;
		}
	}
	return (0);
}

int	ms_run_heredoc(const char *delim, bool quoted, int *out_fd)
{
	int		p[2];
	char	*line;
	size_t	dlen;

	if (pipe(p) < 0)
		return (-1);
	dlen = ft_strlen(delim);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (process_heredoc_line(line, delim, dlen, quoted))
		{
			free(line);
			break ;
		}
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
		free(line);
	}
	close(p[1]);
	*out_fd = p[0];
	return (0);
}
