/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:36:21 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/21 17:36:21 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	count_args_outside_quotes(char *input)
{
	int	i;
	int	count;
	int	in_quote;
	int	in_dquote;
	int	in_arg;

	i = -1;
	count = 0;
	in_quote = 0;
	in_dquote = 0;
	in_arg = 0;
	while (input[++i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (input[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if ((input[i] == ' ' || input[i] == '\t') && !in_quote
			&& !in_dquote && in_arg)
			in_arg = 0;
		else if (!in_arg && ++count)
			in_arg = 1;
	}
	return (count);
}

static void	process_split_char(char **result, char *input, int *vars)
{
	if (input[vars[0]] == '\'' && !vars[4])
		vars[3] = !vars[3];
	else if (input[vars[0]] == '\"' && !vars[3])
		vars[4] = !vars[4];
	if ((input[vars[0]] == ' ' || input[vars[0]] == '\t') && !vars[3]
		&& !vars[4] && vars[5])
	{
		result[vars[2]++] = ft_substr(input, vars[1], vars[0] - vars[1]);
		vars[5] = 0;
	}
	else if (!vars[5])
	{
		vars[1] = vars[0];
		vars[5] = 1;
	}
}

char	**split_args_respect_quotes(char *input)
{
	char	**result;
	int		vars[6];

	vars[2] = count_args_outside_quotes(input);
	result = malloc(sizeof(char *) * (vars[2] + 1));
	if (!result)
		return (NULL);
	vars[0] = -1;
	vars[1] = 0;
	vars[2] = 0;
	vars[3] = 0;
	vars[4] = 0;
	vars[5] = 0;
	while (input[++vars[0]])
		process_split_char(result, input, vars);
	if (vars[5])
		result[vars[2]++] = ft_substr(input, vars[1], vars[0] - vars[1]);
	result[vars[2]] = NULL;
	return (result);
}
