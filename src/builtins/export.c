/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	ms_is_valid_key(const char *s);

static char	**create_rows_array(t_env *env, int *n)
{
	t_env	*cur;
	char	**rows;

	*n = 0;
	cur = env;
	while (cur)
	{
		(*n)++;
		cur = cur->next;
	}
	rows = (char **)malloc(sizeof(char *) * (*n));
	return (rows);
}

static void	fill_rows_array(t_env *env, char **rows)
{
	t_env	*cur;
	int		i;

	i = 0;
	cur = env;
	while (cur)
	{
		if (cur->val)
			rows[i] = create_declare_line_with_val(cur);
		else
			rows[i] = create_declare_line_no_val(cur);
		i++;
		cur = cur->next;
	}
}

static void	print_declare(t_env *env)
{
	int		n;
	int		i;
	char	**rows;

	rows = create_rows_array(env, &n);
	if (!rows)
		return ;
	fill_rows_array(env, rows);
	sort_export_rows(rows, n);
	i = 0;
	while (i < n)
	{
		write(1, rows[i], ft_strlen(rows[i]));
		free(rows[i]);
		i++;
	}
	free(rows);
}

static int	process_export_arg(char *arg, t_env **env)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		if (!ms_is_valid_key(arg))
		{
			print_export_err("export: `", arg, "': not a valid identifier\n");
			*eq = '=';
			return (1);
		}
		ms_export(env, arg, eq + 1);
		*eq = '=';
	}
	else
	{
		if (!ms_is_valid_key(arg))
		{
			print_export_err("export: `", arg, "': not a valid identifier\n");
			return (1);
		}
		ms_export(env, arg, NULL);
	}
	return (0);
}

int	bi_export(char **argv, t_env **env)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	if (!argv[i])
	{
		print_declare(*env);
		return (0);
	}
	while (argv[i])
	{
		if (process_export_arg(argv[i], env))
			ret = 1;
		i++;
	}
	return (ret);
}
