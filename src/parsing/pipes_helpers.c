/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:36:21 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/21 17:36:21 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_pipes_outside_quotes(char *input)
{
	int	i;
	int	count;
	int	in_quote;
	int	in_dquote;

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

int	check_input_helper(char *input, int in_quote, int in_dquote, int i)
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

int	check_input(char *input)
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

void	process_pipe_commands(t_shell *pipe)
{
	int	i;

	i = 0;
	while (pipe->cmds[i])
	{
		pipe->cmds[i] = ft_add_spaces(pipe->cmds[i]);
		replace_spaces_tabs(pipe->cmds[i]);
		dollar_expansion(&pipe->cmds[i], pipe);
		i++;
	}
	pipe->cmd_len = i;
}
