/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static char *join_path(const char *dir, const char *cmd)
{
	char *tmp; char *res;
	tmp = ft_strjoin(dir, "/");
	if (!tmp) return (NULL);
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	return (res);
}

char *ms_resolve_path(const char *cmd, t_env *env)
{
	char    **paths; char *path_env; int i; char *full;

	if (!cmd || !*cmd)
	    return (NULL);
	if (ft_strchr(cmd, '/'))
	    return (ft_strdup(cmd));
	path_env = ms_env_get(env, "PATH");
	if (!path_env)
	    return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths) return (NULL);
	i = 0;
	while (paths[i])
	{
	    full = join_path(paths[i], cmd);
	    if (full && access(full, X_OK) == 0)
	    {
	        int j = 0; while (paths[j]) free(paths[j++]); free(paths);
	        return (full);
	    }
	    free(full);
	    i++;
	}
	i = 0; while (paths[i]) free(paths[i++]); free(paths);
	return (NULL);
}
