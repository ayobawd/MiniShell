/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>

char	*ms_prompt(t_env *env);
int		ms_execute_line(t_cmd *pipeline, t_env **env);
void	ms_cmd_free(t_cmd *pipeline);
void	ms_signals_parent(void);

int	ms_loop(t_env **env)
{
	char	*line;
	char	*p;
	int		status;

	ms_signals_parent();
	status = 0;
	while (1)
	{
		p = ms_prompt(*env);
		line = readline(p);
		free(p);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		status = ms_exec_line_raw(line, env);
		free(line);
	}
	write(1, "exit\n", 5);
	return (status);
}
