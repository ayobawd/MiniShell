/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:45:15 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:59:01 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	quotes_check(char **str, t_variables *v)
{
	if ((*str)[v->i] == '\"')
	{
		if (!v->in_d_quotes)
			v->in_d_quotes = 1;
		else if (v->in_d_quotes)
			v->in_d_quotes = 0;
	}
	if ((*str)[v->i] == '\'' && !v->in_d_quotes)
	{
		if (!v->in_quotes)
			v->in_quotes = 1;
		else if (v->in_quotes)
			v->in_quotes = 0;
	}
}

int	num_of_redirects(char *str)
{
	int			i;
	int			num;
	t_variables	v;

	i = 0;
	num = 0;
	v.i = 0;
	v.in_quotes = 0;
	v.in_d_quotes = 0;
	while (str[i])
	{
		quotes_check(&str, &v);
		if (!v.in_quotes && !v.in_d_quotes && (str[i] == '>' || str[i] == '<'))
		{
			if (str[i + 1] == '>' || str[i + 1] == '<')
				i++;
			num++;
		}
		i++;
		v.i = i;
	}
	return (num);
}
