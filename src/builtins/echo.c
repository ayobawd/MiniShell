/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

static int is_n_flag(const char *s)
{
	size_t i;

	if (!s || s[0] != '-' || s[1] != 'n')
	    return (0);
	i = 2;
	while (s[i])
	{
	    if (s[i] != 'n')
	        return (0);
	    i++;
	}
	return (1);
}

int bi_echo(char **argv)
{
	int i;
	int newline;
	int first;

	i = 1;
	newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{   newline = 0; i++; }
	first = 1;
	while (argv[i])
	{
	    if (!first)
	        write(1, " ", 1);
	    write(1, argv[i], ft_strlen(argv[i]));
	    first = 0;
	    i++;
	}
	if (newline)
	    write(1, "\n", 1);
	return (0);
}
