/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_raw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Helper functions from parsing */
extern int	g_exit_code;

static char	**set_parse_env_from_env(t_shell *ps, t_env *env)
{
	char	**envp;

	envp = ms_env_to_envp(env);
	if (!envp)
		return (NULL);
	copy_env(ps, envp);
	return (envp);
}

static int	validate_and_parse(char *line, t_shell *ps, char **envp)
{
	if (!redirections_parse(line))
	{
		write(2, "syntax error, unexpected redirection token\n", 44);
		ms_status_set(258);
		cleanup_parse_data(ps, envp);
		return (258);
	}
	if (!handle_pipes(ps, line, NULL))
	{
		write(2, "Error\n", 6);
		ms_status_set(258);
		cleanup_parse_data(ps, envp);
		return (258);
	}
	return (0);
}

static void	expand_variables(t_shell *ps)
{
	int	j;

	j = 0;
	while (j < ps->cmd_len)
	{
		dollar_expansion(&ps->cmds[j], ps);
		j++;
	}
}

int	ms_exec_line_raw(char *line, t_env **env)
{
	t_shell	ps;
	t_cmds	*arr;
	int		status;
	char	**envp_for_parse;

	if (!line || spaces(line))
		return (0);
	g_exit_code = ms_status_get();
	ft_bzero(&ps, sizeof(ps));
	envp_for_parse = set_parse_env_from_env(&ps, *env);
	status = validate_and_parse(line, &ps, envp_for_parse);
	if (status != 0)
		return (status);
	expand_variables(&ps);
	files_saving(&ps, &arr);
	status = ms_exec_parsed(arr, ps.cmd_len, env);
	cleanup_parse_data(&ps, envp_for_parse);
	free(arr);
	ms_status_set(status);
	return (status);
}
