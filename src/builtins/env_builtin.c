/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>

int	bi_env(char **argv, t_env **env)
{
	t_env	*cur;

	if (argv[1])
		return (1);
	cur = *env;
	while (cur)
	{
		if (cur->value)
		{
			write(1, cur->key, ft_strlen(cur->key));
			write(1, "=", 1);
			write(1, cur->value, ft_strlen(cur->value));
			write(1, "\n", 1);
		}
		cur = cur->next;
	}
	return (0);
}
