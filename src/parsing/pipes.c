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

static void	split_process_char(char **result, char *input, int *vars)
{
	if (input[vars[1]] == '\'' && !vars[4])
		vars[3] = !vars[3];
	else if (input[vars[1]] == '\"' && !vars[3])
		vars[4] = !vars[4];
	else if (input[vars[1]] == '|' && !vars[3] && !vars[4])
	{
		result[vars[2]++] = ft_substr(input, vars[0], vars[1] - vars[0]);
		vars[0] = vars[1] + 1;
	}
}

static char	**split_pipes_respect_quotes(char *input)
{
	char	**result;
	int		vars[5];

	vars[0] = count_pipes_outside_quotes(input);
	result = malloc(sizeof(char *) * (vars[0] + 2));
	if (!result)
		return (NULL);
	vars[1] = -1;
	vars[2] = 0;
	vars[0] = 0;
	vars[3] = 0;
	vars[4] = 0;
	while (input[++vars[1]])
		split_process_char(result, input, vars);
	result[vars[2]] = ft_substr(input, vars[0], vars[1] - vars[0]);
	result[vars[2] + 1] = NULL;
	return (result);
}

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

static int	pipe_in_quotes(char *input, int i, int quotes, int j)
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
	process_pipe_commands(pipe);
	return (1);
}
