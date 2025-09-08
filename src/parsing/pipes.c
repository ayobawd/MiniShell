/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:13:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 16:22:35 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**quote_aware_pipe_split(char *str)
{
	char	**result;
	int		i, j, start, segments;
	int		in_quote;
	char	quote_char;

	if (!str)
		return (NULL);

	/* Count pipe-separated segments first */
	segments = 1; /* At least one segment */
	i = 0;
	in_quote = 0;
	quote_char = 0;

	while (str[i])
	{
		if (!in_quote && (str[i] == '"' || str[i] == '\''))
		{
			in_quote = 1;
			quote_char = str[i];
		}
		else if (in_quote && str[i] == quote_char)
		{
			in_quote = 0;
			quote_char = 0;
		}
		else if (!in_quote && str[i] == '|')
		{
			segments++;
		}
		i++;
	}

	/* Allocate result array */
	result = malloc(sizeof(char *) * (segments + 1));
	if (!result)
		return (NULL);

	/* Extract segments */
	i = 0;
	j = 0;
	in_quote = 0;
	quote_char = 0;
	start = 0;

	while (j < segments)
	{
		start = i;

		/* Find end of segment (next unquoted pipe or end of string) */
		while (str[i])
		{
			if (!in_quote && (str[i] == '"' || str[i] == '\''))
			{
				in_quote = 1;
				quote_char = str[i];
			}
			else if (in_quote && str[i] == quote_char)
			{
				in_quote = 0;
				quote_char = 0;
			}
			else if (!in_quote && str[i] == '|')
			{
				break; /* Found unquoted pipe, end of this segment */
			}
			i++;
		}

		/* Extract segment */
		result[j] = ft_substr(str, start, i - start);
		if (!result[j])
		{
			while (j > 0)
				free(result[--j]);
			free(result);
			return (NULL);
		}
		j++;

		/* Skip the pipe character for next iteration */
		if (str[i] == '|')
			i++;
	}

	result[j] = NULL;
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
			else if (!quotes)
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
	pipe->cmds = quote_aware_pipe_split(input);
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