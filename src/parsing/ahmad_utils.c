/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ahmad_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:08:42 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 17:47:41 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	increment(char *str, t_variables *vars)
{
	if (str[vars->i] == ' ')
	{
		if (!vars->space_found)
		{
			str[vars->j++] = ' ';
			vars->space_found = 1;
		}
	}
	else
	{
		str[vars->j++] = str[vars->i];
		vars->space_found = 0;
	}
}

static void	process_char(char *str, t_variables *vars)
{
	if (str[vars->i] == '"' || str[vars->i] == '\'')
	{
		if (vars->quote_char == 0)
			vars->quote_char = str[vars->i];
		else if (vars->quote_char == str[vars->i])
			vars->quote_char = 0;
		vars->space_found = 0;
		str[vars->j++] = str[vars->i];
	}
	else if (vars->quote_char == 0)
		increment(str, vars);
	else
	{
		str[vars->j++] = str[vars->i];
		vars->space_found = 0;
	}
}

void	replace_spaces_tabs(char *str)
{
	t_variables	vars;

	vars.i = 0;
	vars.j = 0;
	vars.space_found = 0;
	vars.quote_char = 0;
	while (str[vars.i] != '\0')
	{
		process_char(str, &vars);
		vars.i++;
	}
	str[vars.j] = '\0';
}

void	clean_quotes(char *str)
{
	int	i;
	int	j;
	int	quote;

	j = 0;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!quote)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
			else
				str[j++] = str[i];
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}
