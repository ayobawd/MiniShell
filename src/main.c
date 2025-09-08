/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

volatile sig_atomic_t g_sig = 0;

int	ms_loop(t_env **env);
t_env	*ms_env_from_environ(char **environ);
void	ms_env_free(t_env *env);

int	main(int argc, char **argv, char **environ)
{
	t_env   *env;
	int     status;

	(void)argc;
	(void)argv;
	env = ms_env_from_environ(environ);
	if (!env)
	    return (1);
	status = ms_loop(&env);
	ms_env_free(env);
	return (status);
}

