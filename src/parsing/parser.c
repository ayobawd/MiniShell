/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:44:29 by aradwan           #+#    #+#             */
/*   Updated: 2025/08/15 21:17:36 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exit_code = 0;

int	parsing(t_shell *pipe, t_cmds *cmds, char *input)
{
	if (spaces(input))
		return (1);
	if (!redirections_parse(input))
	{
		printf("syntax error, unexpected redirection token\n");
		add_history(input);
		exit_code = 258;
		return (1);
	}
	else if (!handle_pipes(pipe, input, cmds))
	{
		printf("Error\n");
		add_history(input);
		exit_code = 258;
		return (1);
	}
	return (0);
}
