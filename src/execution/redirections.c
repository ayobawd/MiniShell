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

int	setup_redirections(t_cmds *cmd)
{
	int	i;
	int	fd;

	if (!cmd || !cmd->outs)
		return (0);
	i = 0;
	while (i < cmd->red_len)
	{
		if (cmd->outs[i].flag == IN_FILE)
		{
			fd = open(cmd->outs[i].file_name, O_RDONLY);
			if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
				return (perror(cmd->outs[i].file_name), close(fd), -1);
			close(fd);
		}
		else if (cmd->outs[i].flag == OUT_FILE)
		{
			fd = open(cmd->outs[i].file_name,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
				return (perror(cmd->outs[i].file_name), close(fd), -1);
			close(fd);
		}
		else if (cmd->outs[i].flag == APPEND)
		{
			fd = open(cmd->outs[i].file_name,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
				return (perror(cmd->outs[i].file_name), close(fd), -1);
			close(fd);
		}
		else if (cmd->outs[i].flag == HERE_DOC
			&& setup_heredoc(cmd->outs[i].file_name) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	setup_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;
	int		delim_len;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
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
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
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
