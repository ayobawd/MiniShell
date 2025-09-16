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

static void	process_input_interactive(t_shell *s, char *input)
{
	t_cmds	*cmd;

	cmd = NULL;
	if (parsing(s, cmd, input))
	{
		free(input);
		return ;
	}
	init_commands(s, &cmd);
	g_exit_code = execute_commands(s, cmd);
	free_all(s, cmd);
	add_history(input);
	free(input);
}

static void	process_input(t_shell *s, char *input)
{
	t_cmds	*cmd;

	cmd = NULL;
	if (parsing(s, cmd, input))
	{
		free(input);
		return ;
	}
	init_commands(s, &cmd);
	g_exit_code = execute_commands(s, cmd);
	free_all(s, cmd);
	free(input);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_shell		s;

	copy_env(&s, env);
	s.cmd_len = 0;
	s.cmds = NULL;
	if (ac == 3 && ft_strncmp(av[1], "-c", 3) == 0)
	{
		process_input(&s, ft_strdup(av[2]));
		return (g_exit_code);
	}
	if (ac != 1)
		return (0);
	while (1)
	{
		signal(SIGINT, handle_signals);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell> ");
		if (!input)
			return (printf("exit\n"), 0);
		process_input_interactive(&s, input);
	}
	return (0);
}
