/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:23:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 17:24:16 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_status_get(void);

int	handle_exit_code(char **str, t_variables *var)
{
	char	*code;

	code = ft_itoa(ms_status_get());
	if (!code)
		return (1);
	*str = storing(*str, var->i - 1, 2, code);
	var->i += ft_strlen(code) - 2;
	free(code);
	return (1);
}

int	handle_brace_syntax(char **str, t_variables *var)
{
	var->i++;
	var->len = 0;
	while ((*str)[var->i] && (*str)[var->i] != '}'
		&& (ft_isalnum((unsigned char)(*str)[var->i])
		|| (*str)[var->i] == '_'))
	{
		var->len++;
		var->i++;
	}
	if ((*str)[var->i] == '}')
	{
		var->i++;
		var->len += 2;
		return (0);
	}
	return (1);
}

void	handle_regular_variable(char **str, t_variables *var)
{
	var->len = 0;
	while (ft_isalnum((unsigned char)(*str)[var->i]) || (*str)[var->i] == '_')
	{
		var->len++;
		var->i++;
	}
}

void	init_variables(t_variables *var, char **expanded)
{
	var->len = 0;
	*expanded = NULL;
	var->i = 0;
	var->in_d_quotes = 0;
	var->in_quotes = 0;
}
