/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spaces_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 14:31:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 16:14:59 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_space(int *j, char **new_str, int var)
{
	(*new_str)[(*j)++] = ' ';
	if (var == 0)
		(*new_str)[(*j)++] = '<';
	else if (var == 1)
	{
		(*new_str)[(*j)++] = '<';
		(*new_str)[(*j)++] = '<';
	}
	else if (var == 2)
		(*new_str)[(*j)++] = '>';
	else if (var == 3)
	{
		(*new_str)[(*j)++] = '>';
		(*new_str)[(*j)++] = '>';
	}
	else if (var == 4)
		(*new_str)[(*j)++] = ' ';
	(*new_str)[(*j)++] = ' ';
}

static void	add_spaces_helper(char **input, int *i, int *j, char **new_input)
{
	if ((*input)[*i] == '<' && (*input)[*i + 1] != '<')
		add_space(j, new_input, 0);
	else if ((*input)[*i] == '<' && (*input)[*i + 1] == '<')
	{
		(*i)++;
		add_space(j, new_input, 1);
	}
	else if ((*input)[*i] == '>' && (*input)[*i + 1] != '>')
		add_space(j, new_input, 2);
	else if ((*input)[*i] == '>' && (*input)[*i + 1] == '>')
	{
		(*i)++;
		add_space(j, new_input, 3);
	}
	else if ((*input)[*i] == '\t')
		add_space(j, new_input, 4);
	else
		(*new_input)[(*j)++] = (*input)[(*i)];
}

char	*ft_add_spaces(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	int		single_q;
	int		double_q;

	i = -1;
	j = 0;
	single_q = 0;
	double_q = 0;
	new_input = malloc(ft_strlen(input) * 3 + 2);
	while (input[++i])
	{
		if (input[i] == '\'' && double_q == 0)
			single_q = !single_q;
		else if (input[i] == '\"' && single_q == 0)
			double_q = !double_q;
		if (!single_q && !double_q)
			add_spaces_helper(&input, &i, &j, &new_input);
		else
			new_input[j++] = input[i];
	}
	new_input[j] = '\0';
	free(input);
	return (new_input);
}
