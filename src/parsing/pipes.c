/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:08:56 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/21 16:51:32 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

static int count_pipes_outside_quotes(char *input)
{
	int i;
	int count;
	int in_quote;
	int in_dquote;

	i = 0;
	count = 0;
	in_quote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (input[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if (input[i] == '|' && !in_quote && !in_dquote)
			count++;
		i++;
	}
	return (count);
}

static char **split_pipes_respect_quotes(char *input)
{
	char **result;
	int pipe_count;
	int i, start, cmd_idx;
	int in_quote, in_dquote;

	pipe_count = count_pipes_outside_quotes(input);
	result = malloc(sizeof(char *) * (pipe_count + 2));
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	cmd_idx = 0;
	in_quote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (input[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if (input[i] == '|' && !in_quote && !in_dquote)
		{
			result[cmd_idx] = ft_substr(input, start, i - start);
			cmd_idx++;
			start = i + 1;
		}
		i++;
	}
	result[cmd_idx] = ft_substr(input, start, i - start);
	result[cmd_idx + 1] = NULL;
	return (result);
}

static int pipe_from_back(char *input)
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

static int pipe_in_quotes(char *input, int i, int quotes, int j)
{
    while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			if (quotes == 0)
				quotes = input[i];
			else if (quotes == input[i])
				quotes = 0;
			i++;
			continue ;
		}
		if (input[i] == '|' && !quotes)
		{
			j = i + 1;
			while (input[j] == ' ' || input[j] == '\t')
				j++;
			if (input[j] == '\0' || input[j] == '|')
				return (0);
		}
		i++;
	}
	return (1);
}

static int	check_input_helper(char *input, int in_quote, int in_dquote, int i)
{
	while (input[i])
	{
		if (input[i] == '\'')
		{
			if (in_quote)
				in_quote = 0;
			else if (!in_dquote)
				in_quote = 1;
		}
		else if (input[i] == '\"')
		{
			if (in_dquote)
				in_dquote = 0;
			else if (!in_quote)
				in_dquote = 1;
		}
		i++;
	}
	if (in_quote || in_dquote)
		return (0);
	return (1);
}

static int	check_input(char *input)
{
	int	i;
	int	in_quote;
	int	in_d_quote;

	i = 0;
	in_quote = 0;
	in_d_quote = 0;
	if (!check_input_helper(input, in_quote, in_d_quote, i))
		return (0);
	return (1);
}

int	handle_pipes(t_shell *pipe, char *input, t_cmds *cmds)
{
	int	i;
	int	j;
	int	quotes;

	(void)cmds;
	quotes = 0;
	i = 0;
	j = 0;
	if (pipe_from_back(input))
		return (0);
	if (!pipe_in_quotes(input, i, quotes, j))
		return (0);
	if (!check_input(input))
		return (0);
	pipe->cmds = split_pipes_respect_quotes(input);
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