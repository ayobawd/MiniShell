/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:35:26 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:58:35 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	handle_redirections(char **input, t_variables *v)
{
	char	*str;

	str = *input;
	if (!check_last(str))
		return (0);
	v->i = 0;
	while (str[v->i])
	{
		quotes_check(&str, v);
		if (!is_redirect(input, v))
			return (0);
		v->i++;
	}
	return (1);
}

int	redirections_parse(char *str)
{
	t_variables	v;

	v.i = 0;
	v.indx = 0;
	v.in_d_quotes = 0;
	v.in_quotes = 0;
	if (!handle_redirections(&str, &v))
		return (0);
	return (1);
}
