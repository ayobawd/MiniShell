/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int	ms_status_get(void);

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	print_exit_error(char *arg)
{
	write(2, "exit: ", 6);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
}

static int	handle_exit_args(char **argv, int ac, bool in_parent)
{
	int	status;

	if (!is_valid_number(argv[1]))
	{
		print_exit_error(argv[1]);
		if (in_parent)
			exit(2);
		return (2);
	}
	if (ac > 2)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	status = ft_atoi(argv[1]) & 0xFF;
	if (in_parent)
		exit(status);
	return (status);
}

int	bi_exit(char **argv, t_env **env, bool in_parent)
{
	int	ac;
	int	status;

	(void)env;
	ac = 0;
	while (argv[ac])
		ac++;
	write(1, "exit\n", 5);
	if (ac == 1)
	{
		status = ms_status_get();
		if (in_parent)
			exit(status);
		return (status);
	}
	return (handle_exit_args(argv, ac, in_parent));
}
