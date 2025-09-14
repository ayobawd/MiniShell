/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:13:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:57:00 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	pipe_from_back(char *input)
{
	int	len;
	int	i;

	len = ft_strlen(input);
	if (!len)
		return (0);
	len--;
	while ((input[len] == ' ' || input[len] == '\t') && len > 0)
		len--;
	if (input[len] == '|')
		return (1);
	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

static int	pipe_in_quotes(char *input, int i, int j)
{
	t_variables	v;

	v.i = 0;
	v.in_quotes = 0;
	v.in_d_quotes = 0;
	while (input[i])
	{
		quotes_check(&input, &v);
		if (input[i] == '|' && !v.in_quotes && !v.in_d_quotes)
		{
			j = i + 1;
			while (input[j] == ' ' || input[j] == '\t')
				j++;
			if (input[j] == '\0' || input[j] == '|')
				return (0);
		}
		i++;
		v.i = i;
	}
	return (1);
}

static int	check_input_helper(char *input, int i)
{
	t_variables	v;

	v.i = 0;
	v.in_quotes = 0;
	v.in_d_quotes = 0;
	while (input[i])
	{
		quotes_check(&input, &v);
		i++;
		v.i = i;
	}
	if (v.in_quotes || v.in_d_quotes)
		return (0);
	return (1);
}

static int	check_input(char *input)
{
	int	i;

	i = 0;
	if (!check_input_helper(input, i))
		return (0);
	return (1);
}

int	handle_pipes(t_shell *pipe, char *input, t_cmds *cmds)
{
	int	i;
	int	j;

	(void)cmds;
	i = 0;
	j = 0;
	if (pipe_from_back(input))
		return (0);
	if (!pipe_in_quotes(input, i, j))
		return (0);
	if (!check_input(input))
		return (0);
	pipe->cmds = ft_split(input, '|');
	i = 0;
	while (pipe->cmds[i])
	{
		pipe->cmds[i] = ft_add_spaces(pipe->cmds[i]);
		replace_spaces_tabs(pipe->cmds[i]);
		i++;
	}
	pipe->cmd_len = i;
	return (1);
}
