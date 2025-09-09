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