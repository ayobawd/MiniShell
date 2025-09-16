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

static void	read_heredoc_input(int write_fd, char *delimiter)
{
	char	*line;
	int		delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

int	setup_heredoc(char *delimiter)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	read_heredoc_input(pipefd[1], delimiter);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

int	restore_std_fds(void)
{
	static int	saved_stdin = -1;
	static int	saved_stdout = -1;

	if (saved_stdin == -1)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
	}
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		return (-1);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (-1);
	return (0);
}
