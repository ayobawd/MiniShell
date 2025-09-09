/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 13:13:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 16:22:35 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	num_of_redirects(char *str)
{
	int	i;
	int	num;
	int	in_quote;

	i = 0;
	num = 0;
	in_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!in_quote)
				in_quote = str[i];
			else if (in_quote == str[i])
				in_quote = 0;
		}
		else if (!in_quote && (str[i] == '>' || str[i] == '<'))
		{
			if (str[i + 1] == '>' || str[i + 1] == '<')
				i++;
			num++;
		}
		i++;
	}
	return (num);
}

void	remove_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;

	if (!s)
		return ;
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && i < start + len)
		{
			i++;
			continue ;
		}
		s[j] = s[i];
		i++;
		j++;
	}
	s[j] = '\0';
}

int	is_spacee(int c)
{
	return (c == ' ' || c == '\t');
}

int	spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_spacee(str[i]))
			return (0);
		i++;
	}
	return (1);
}
