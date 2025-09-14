/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:08:42 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:59:05 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	is_spacee(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static void	increment(char *str, t_variables *v)
{
	if (str[v->i] == ' ')
	{
		if (!v->space_found)
		{
			str[v->indx++] = ' ';
			v->space_found = 1;
		}
	}
	else
	{
		str[v->indx++] = str[v->i];
		v->space_found = 0;
	}
}

void	replace_spaces_tabs(char *str)
{
	t_variables	v;

	v.i = 0;
	v.indx = 0;
	v.space_found = 0;
	v.quote_char = 0;
	while (str[v.i] != '\0')
	{
		quotes_check(&str, &v);
		if (v.quote_char == 0)
			increment(str, &v);
		else
			str[v.indx++] = str[v.i];
		v.i++;
	}
	str[v.indx] = '\0';
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
