/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:23:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 17:24:16 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand(char **str, char *expanded, t_variables *var)
{
	(void)expanded;
	var->i++;
	if ((*str)[var->i] == '?')
		return (handle_exit_code(str, var));
	if ((*str)[var->i] == '{')
		return (handle_brace_syntax(str, var));
	if (!ft_isalpha((unsigned char)(*str)[var->i]) && (*str)[var->i] != '_')
		return (1);
	handle_regular_variable(str, var);
	return (0);
}

static char	*get_variable_substring(char **str, t_variables *var,
	int brace_syntax)
{
	int	var_start;

	if (brace_syntax)
	{
		var_start = var->i - var->len + 1;
		return (ft_substr(*str, var_start, var->len - 2));
	}
	else
	{
		return (ft_substr(*str, var->i - var->len, var->len));
	}
}

static int	process_dollar_sign(char **str, t_variables *var, t_shell *pipe,
	char *expanded)
{
	int		brace_syntax;
	char	*tmp;

	brace_syntax = 0;
	if ((*str)[var->i + 1] == '{')
		brace_syntax = 1;
	if (expand(str, expanded, var))
		return (1);
	tmp = get_variable_substring(str, var, brace_syntax);
	if (generate_string(str, &tmp, var, pipe))
		return (1);
	return (0);
}

void	dollar_expansion(char **str, t_shell *pipe)
{
	t_variables	var;
	char		*expanded;

	init_variables(&var, &expanded);
	while ((*str)[var.i])
	{
		quotes_check(str, &var);
		if ((*str)[var.i] == '$' && !var.in_quotes && (*str)[var.i + 1])
		{
			if (process_dollar_sign(str, &var, pipe, expanded))
				continue ;
		}
		var.i++;
	}
}
