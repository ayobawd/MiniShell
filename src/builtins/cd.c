/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

static void update_pwd_vars(t_env **env, const char *oldpwd)
{
    char    buf[4096];
    if (oldpwd)
        ms_export(env, "OLDPWD", oldpwd);
    if (getcwd(buf, sizeof(buf)))
        ms_export(env, "PWD", buf);
}

int bi_cd(char **argv, t_env **env)
{
    const char  *path;
    char        oldpwd[4096];
    char        *home;

    if (argv[1] && argv[2])
    {
        write(2, "cd: too many arguments\n", 24);
        return (1);
    }
    if (!getcwd(oldpwd, sizeof(oldpwd)))
        oldpwd[0] = '\0';
    if (!argv[1])
    {
        home = ms_env_get(*env, "HOME");
        if (!home)
        {
            write(2, "cd: HOME not set\n", 17);
            return (1);
        }
        path = home;
    }
    else
        path = argv[1];
    if (chdir(path) != 0)
    {
        write(2, "cd: ", 4);
        write(2, path, ft_strlen(path));
        write(2, ": No such file or directory\n", 28);
        return (1);
    }
    update_pwd_vars(env, oldpwd);
    return (0);
}
