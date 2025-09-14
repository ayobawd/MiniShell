/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include "../../minishell.h"

int	bi_pwd(char **argv)
{
	char	buf[PATH_MAX];

	(void)argv;
	if (getcwd(buf, sizeof(buf)))
	{
		write(1, buf, ft_strlen(buf));
		write(1, "\n", 1);
		return (0);
	}
	return (1);
}
