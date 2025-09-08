/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ms_status_get(void);
void	ms_status_set(int st);

int	ms_is_builtin(const char *name)
{
	if (!name)
		return (0);
	if (!ft_strncmp(name, "echo", 4) && name[4] == '\0')
		return (1);
	if (!ft_strncmp(name, "cd", 2) && name[2] == '\0')
		return (1);
	if (!ft_strncmp(name, "pwd", 3) && name[3] == '\0')
		return (1);
	if (!ft_strncmp(name, "export", 6) && name[6] == '\0')
		return (1);
	if (!ft_strncmp(name, "unset", 5) && name[5] == '\0')
		return (1);
	if (!ft_strncmp(name, "env", 3) && name[3] == '\0')
		return (1);
	if (!ft_strncmp(name, "exit", 4) && name[4] == '\0')
		return (1);
	return (0);
}

int	ms_run_builtin(char **argv, t_env **env, bool in_parent)
{
	if (!ft_strncmp(argv[0], "echo", 4) && argv[0][4] == '\0')
		return (bi_echo(argv));
	if (!ft_strncmp(argv[0], "cd", 2) && argv[0][2] == '\0')
		return (bi_cd(argv, env));
	if (!ft_strncmp(argv[0], "pwd", 3) && argv[0][3] == '\0')
		return (bi_pwd(argv));
	if (!ft_strncmp(argv[0], "export", 6) && argv[0][6] == '\0')
		return (bi_export(argv, env));
	if (!ft_strncmp(argv[0], "unset", 5) && argv[0][5] == '\0')
		return (bi_unset(argv, env));
	if (!ft_strncmp(argv[0], "env", 3) && argv[0][3] == '\0')
		return (bi_env(argv, env));
	if (!ft_strncmp(argv[0], "exit", 4) && argv[0][4] == '\0')
		return (bi_exit(argv, env, in_parent));
	return (1);
}

static bool	is_parent_builtin(const char *cmd)
{
	if (!ft_strncmp(cmd, "cd", 2) && cmd[2] == '\0')
		return (true);
	if (!ft_strncmp(cmd, "export", 6) && cmd[6] == '\0')
		return (true);
	if (!ft_strncmp(cmd, "unset", 5) && cmd[5] == '\0')
		return (true);
	if (!ft_strncmp(cmd, "exit", 4) && cmd[4] == '\0')
		return (true);
	return (false);
}

int	ms_execute_line(t_cmd *pipeline, t_env **env)
{
	int		status;
	t_cmd	*single;

	if (!pipeline)
		return (0);
	single = pipeline;
	if (!single->next && single->argv && single->argv[0]
		&& ms_is_builtin(single->argv[0])
		&& is_parent_builtin(single->argv[0]))
	{
		status = ms_run_builtin(single->argv, env, true);
		ms_status_set(status);
		return (status);
	}
	status = ms_exec_pipeline(pipeline, env);
	ms_status_set(status);
	return (status);
}
