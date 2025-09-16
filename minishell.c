/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:25:01 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 16:03:09 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_shell		s;
	t_cmds		*cmd;

	(void)av;
	if (ac != 1)
		return (0);
	copy_env(&s, env);
	s.cmd_len = 0;
	s.cmds = NULL;
	while (1)
	{
		signal(SIGINT, handle_signals);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell> ");
		if (!input)
			return (printf("exit\n"), 0);
		cmd = NULL;
		if (parsing(&s, cmd, input))
		{
			free(input);
			continue ;
		}
		init_commands(&s, &cmd);
		/* Parsing-only mode: commands are prepared but not executed */
		free_all(&s, cmd);
		add_history(input);
		free(input);
	}
	return (0);
}
