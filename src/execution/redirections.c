/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_input_redirection(t_redirect redir)
{
	int	fd;

	fd = open(redir.file_name, O_RDONLY);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
		return (perror(redir.file_name), close(fd), -1);
	close(fd);
	return (0);
}

static int	handle_output_redirection(t_redirect redir)
{
	int	fd;

	if (redir.flag == OUT_FILE)
		fd = open(redir.file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir.file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
		return (perror(redir.file_name), close(fd), -1);
	close(fd);
	return (0);
}

static int	handle_single_redirection(t_redirect redir)
{
	if (redir.flag == IN_FILE)
		return (handle_input_redirection(redir));
	else if (redir.flag == OUT_FILE || redir.flag == APPEND)
		return (handle_output_redirection(redir));
	else if (redir.flag == HERE_DOC && setup_heredoc(redir.file_name) == -1)
		return (-1);
	return (0);
}

int	setup_redirections(t_cmds *cmd)
{
	int	i;

	if (!cmd || !cmd->outs)
		return (0);
	i = 0;
	while (i < cmd->red_len)
	{
		if (handle_single_redirection(cmd->outs[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}


