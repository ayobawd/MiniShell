/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int ms_is_valid_key(const char *s);

int bi_unset(char **argv, t_env **env)
{
    int i;
    int ret;

    i = 1;
    ret = 0;
    while (argv[i])
    {
        if (!ms_is_valid_key(argv[i]))
        {
            write(2, "unset: `", 8);
            write(2, argv[i], ft_strlen(argv[i]));
            write(2, "': not a valid identifier\n", 26);
            ret = 1;
        }
        else
            ms_unset(env, argv[i]);
        i++;
    }
    return (ret);
}
