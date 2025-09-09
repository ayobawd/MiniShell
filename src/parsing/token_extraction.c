/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	extract_single_token(char *str, int *i, t_variables *v)
{
	while (str[*i])
	{
		if (!v->in_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			v->in_quotes = 1;
			v->quote_char = str[*i];
		}
		else if (v->in_quotes && str[*i] == v->quote_char)
		{
			v->in_quotes = 0;
			v->quote_char = 0;
		}
		else if (!v->in_quotes && (str[*i] == ' ' || str[*i] == '\t'))
			break ;
		(*i)++;
	}
}

static char	**alloc_and_init_tokens(int tokens, int *i, int *j, t_variables *v)
{
	char	**result;

	result = malloc(sizeof(char *) * (tokens + 1));
	if (!result)
		return (NULL);
	*i = 0;
	*j = 0;
	v->in_quotes = 0;
	v->quote_char = 0;
	return (result);
}

static void	handle_alloc_failure(char **result, int j)
{
	while (j > 0)
		free(result[--j]);
	free(result);
}

static int	process_token(char *str, char **result, int *indices,
		t_variables *v)
{
	int	start;

	while (str[indices[0]] == ' ' || str[indices[0]] == '\t')
		indices[0]++;
	if (!str[indices[0]])
		return (0);
	start = indices[0];
	extract_single_token(str, &indices[0], v);
	result[indices[1]] = ft_substr(str, start, indices[0] - start);
	if (!result[indices[1]])
	{
		handle_alloc_failure(result, indices[1]);
		return (-1);
	}
	indices[1]++;
	return (1);
}

char	**extract_tokens(char *str, int tokens)
{
	char		**result;
	int			indices[2];
	t_variables	v;
	int			status;

	result = alloc_and_init_tokens(tokens, &indices[0], &indices[1], &v);
	if (!result)
		return (NULL);
	while (str[indices[0]] && indices[1] < tokens)
	{
		status = process_token(str, result, indices, &v);
		if (status == -1)
			return (NULL);
		if (status == 0)
			break ;
	}
	result[indices[1]] = NULL;
	return (result);
}
