/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:13:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 16:22:35 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_pipe_segments(char *str)
{
	int		i;
	int		segments;
	char	quote_char;

	if (!str)
		return (0);
	segments = 1;
	i = 0;
	quote_char = 0;
	while (str[i])
	{
		if (!quote_char && (str[i] == '"' || str[i] == '\''))
			quote_char = str[i];
		else if (quote_char && str[i] == quote_char)
			quote_char = 0;
		else if (!quote_char && str[i] == '|')
			segments++;
		i++;
	}
	return (segments);
}

int	find_segment_end(char *str, int start, char *quote_char)
{
	int	i;

	i = start;
	while (str[i])
	{
		if (!*quote_char && (str[i] == '"' || str[i] == '\''))
			*quote_char = str[i];
		else if (*quote_char && str[i] == *quote_char)
			*quote_char = 0;
		else if (!*quote_char && str[i] == '|')
			break ;
		i++;
	}
	return (i);
}

static void	cleanup_segments(char **result, int j)
{
	while (j > 0)
		free(result[--j]);
	free(result);
}

char	**extract_segments(char *str, int segments)
{
	char	**result;
	int		i;
	int		j;
	int		end;
	char	quote_char;

	result = malloc(sizeof(char *) * (segments + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote_char = 0;
	while (j < segments)
	{
		end = find_segment_end(str, i, &quote_char);
		result[j] = ft_substr(str, i, end - i);
		if (!result[j])
			return (cleanup_segments(result, j), NULL);
		i = end + (str[end] == '|');
		j++;
	}
	result[j] = NULL;
	return (result);
}

int	pipe_in_quotes(char *input, int i, int quotes, int j)
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
