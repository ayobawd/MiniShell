/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:25:01 by aradwan           #+#    #+#             */
/*   Updated: 2025/10/11 10:37:26 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_input(t_shell *s, char *input, t_cmds **cmd)
{
	if (parsing(s, *cmd, input))
		return (1);
	init_commands(s, cmd);
	s->current_cmds = *cmd;
	g_exit_code = execute_commands(s, *cmd);
	free_all(s, *cmd);
	s->current_cmds = NULL;
	add_history(input);
	return (0);
}

static void	shell_loop(t_shell *s)
{
	char	*input;
	t_cmds	*cmd;

	while (1)
	{
		setup_signals_interactive();
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			free_environment(s);
			rl_clear_history();
			return ;
		}
		cmd = NULL;
		process_input(s, input, &cmd);
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	s;

	(void)av;
	if (ac != 1)
		return (0);
	copy_env(&s, env);
	s.cmd_len = 0;
	s.cmds = NULL;
	s.current_cmds = NULL;
	shell_loop(&s);
	free_environment(&s);
	rl_clear_history();
	return (0);
}
