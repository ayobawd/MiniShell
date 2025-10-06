/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:00:00 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/12/22 12:00:00 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_space_tab(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_sign(char c)
{
	return (c == '+' || c == '-');
}

static int	parse_exit_number(const char *s, int *out)
{
	int	i;
	int	has_digit;

	i = 0;
	while (s[i] && is_space_tab(s[i]))
		i++;
	if (s[i] && is_sign(s[i]))
		i++;
	has_digit = (s[i] && ft_isdigit(s[i]));
	if (!has_digit)
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (s[i] != '\0')
		return (0);
	*out = ft_atoi(s);
	return (1);
}

static void	cleanup_before_exit(void)
{
	int	fd;

	fd = 3;
	while (fd < 10)
	{
		close(fd);
		fd++;
	}
}

int	builtin_exit(t_cmds *cmd)
{
	int	exit_code;

	printf("exit\n");
	if (!cmd->cmds[1])
	{
		cleanup_before_exit();
		exit(0);
	}
	if (!parse_exit_number(cmd->cmds[1], &exit_code))
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		cleanup_before_exit();
		exit(2);
	}
	if (cmd->cmds[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	cleanup_before_exit();
	exit(exit_code);
	return (0);
}
