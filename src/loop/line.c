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

char    *ms_prompt(t_env *env);
int          ms_execute_line(t_cmd *pipeline, t_env **env);
void         ms_cmd_free(t_cmd *pipeline);
void         ms_signals_parent(void);

int ms_loop(t_env **env)
{
    char    *line;
    t_cmd   *pipeline;
    int     status;

    ms_signals_parent();
    status = 0;
    while (1)
    {
        if (isatty(0))
        {
            char *p = ms_prompt(*env);
            line = readline(p);
            free(p);
        }
        else
            line = readline(NULL);
        if (!line)
            break ;
        if (*line)
            add_history(line);
        pipeline = parse_to_pipeline(line, *env);
        free(line);
        if (pipeline)
        {
            status = ms_execute_line(pipeline, env);
            ms_cmd_free(pipeline);
        }
    }
    write(1, "exit\n", 5);
    return (status);
}
