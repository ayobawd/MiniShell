/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
