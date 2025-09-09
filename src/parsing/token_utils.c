/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	skip_whitespace(char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
}

static void	handle_quote_transition(char *str, int *i, int *in_quote,
		char *quote_char)
{
	if (!*in_quote && (str[*i] == '"' || str[*i] == '\''))
	{
		*in_quote = 1;
		*quote_char = str[*i];
	}
	else if (*in_quote && str[*i] == *quote_char)
	{
		*in_quote = 0;
		*quote_char = 0;
	}
}

static int	should_continue_token(char *str, int i, int in_quote)
{
	if (!str[i])
		return (0);
	if (in_quote)
		return (1);
	if (str[i] == ' ' || str[i] == '\t')
		return (0);
	return (1);
}

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
		skip_whitespace(str, &i);
		if (!str[i])
			break ;
		tokens++;
		while (should_continue_token(str, i, in_quote))
		{
			handle_quote_transition(str, &i, &in_quote, &quote_char);
			i++;
		}
	}
	return (tokens);
}
