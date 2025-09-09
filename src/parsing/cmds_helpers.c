/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_tokens(char *str)
{
	int		i;
	int		tokens;
	int		in_quote;
	char	quote_char;

	tokens = 0;
	i = 0;
	in_quote = 0;
	quote_char = 0;
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break ;
		tokens++;
		while (str[i] && ((in_quote && str[i] != quote_char)
				|| (!in_quote && str[i] != ' ' && str[i] != '\t')))
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
			i++;
		}
	}
	return (tokens);
}

char	**extract_tokens(char *str, int tokens)
{
	char		**result;
	int			i;
	int			j;
	int			start;
	t_variables	v;

	result = malloc(sizeof(char *) * (tokens + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	v.in_quotes = 0;
	v.quote_char = 0;
	while (str[i] && j < tokens)
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break ;
		start = i;
		while (str[i])
		{
			if (!v.in_quotes && (str[i] == '"' || str[i] == '\''))
			{
				v.in_quotes = 1;
				v.quote_char = str[i];
			}
			else if (v.in_quotes && str[i] == v.quote_char)
			{
				v.in_quotes = 0;
				v.quote_char = 0;
			}
			else if (!v.in_quotes && (str[i] == ' ' || str[i] == '\t'))
				break ;
			i++;
		}
		result[j] = ft_substr(str, start, i - start);
		if (!result[j])
		{
			while (j > 0)
				free(result[--j]);
			free(result);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}