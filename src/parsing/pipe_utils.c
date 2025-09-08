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

char	**extract_segments(char *str, int segments)
{
	char	**result;
	int		i;
	int		j;
	char	quote_char;

	result = malloc(sizeof(char *) * (segments + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote_char = 0;
	while (j < segments)
	{
		result[j] = ft_substr(str, i,
				find_segment_end(str, i, &quote_char) - i);
		if (!result[j])
		{
			while (j > 0)
				free(result[--j]);
			free(result);
			return (NULL);
		}
		i = find_segment_end(str, i, &quote_char);
		if (str[i] == '|')
			i++;
		j++;
	}
	result[j] = NULL;
	return (result);
}

int	pipe_from_back(char *input)
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