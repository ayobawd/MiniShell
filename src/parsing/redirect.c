/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:35:26 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/21 16:51:35 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_redirect(char **input, t_variables *v)
{
	if ((*input)[v->i] == '>' || (*input)[v->i] == '<')
	{
		v->i++;
		if (((*input)[v->i] == '<' && (*input)[v->i - 1] == '<') || \
((*input)[v->i] == '>' && (*input)[v->i - 1] == '>'))
			v->i++;
		while ((*input)[v->i] == ' ' || (*input)[v->i] == '\t')
			v->i++;
		if (((*input)[v->i] == '>' || (*input)[v->i] == '<') \
&& (!v->in_quotes))
			return (0);
		while ((*input)[v->i] == ' ' || (*input)[v->i] == '\t')
			v->i++;
		if ((*input)[v->i] == '\0' || ((*input)[v->i] == '|' && !v->in_quotes))
			return (0);
	}
	else
		v->i++;
	return (1);
}

static int	check_last(char *input)
{
	int	i;

	i = ft_strlen(input);
	if (i)
		i--;
	while (is_spacee(input[i]) && i > 0)
		i--;
	if (input[i] == '>' || input[i] == '<')
		return (0);
	return (1);
}

static int	handle_redirections(char **str, t_variables *vars)
{
	if (!check_last(*str))
		return (0);
	vars->i = 0;
	while ((*str)[vars->i])
	{
		if ((*str)[vars->i] == '\'' || (*str)[vars->i] == '\"')
		{
			if (!vars->in_quotes)
				vars->in_quotes = (*str)[vars->i];
			else if (vars->in_quotes == (*str)[vars->i])
				vars->in_quotes = 0;
		}
		if (!is_redirect(str, vars))
			return (0);
	}
	return (1);
}

int	redirections_parse(char *str)
{
	t_variables	v;

	v.i = 0;
	v.j = 0;
	v.in_d_quotes = 0;
	v.in_quotes = 0;
	if (!handle_redirections(&str, &v))
		return (0);
	return (1);
}
