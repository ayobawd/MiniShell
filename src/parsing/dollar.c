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

char	*storing(char *str, int start, int len, char *replace)
{
	char	*new_str;
	int		i;
	int		j;

	if (!str || start + len > (int)ft_strlen(str))
		return (NULL);
	new_str = malloc(sizeof(char)
			* (ft_strlen(str) - len + ft_strlen(replace) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (i < start)
		new_str[j++] = str[i++];
	while (*replace)
		new_str[j++] = *replace++;
	while (str[i + len])
	{
		new_str[j] = str[i + len];
		i++;
		j++;
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

void	quotes_check(char **str, t_variables *var)
{
	if ((*str)[var->i] == '\"')
	{
		if (!var->in_d_quotes)
			var->in_d_quotes = 1;
		else if (var->in_d_quotes)
			var->in_d_quotes = 0;
	}
	if ((*str)[var->i] == '\'' && !var->in_d_quotes)
	{
		if (!var->in_quotes)
			var->in_quotes = 1;
		else if (var->in_quotes)
			var->in_quotes = 0;
	}
}

int	expand(char **str, char *expanded, t_variables *var)
{
	char	*code;

	(void)expanded;
	var->i++;
	if ((*str)[var->i] == '?')
	{
		code = ft_itoa(g_exit_code);
		if (!code)
			return (1);
		*str = storing(*str, var->i - 1, 2, code);
		var->i += ft_strlen(code) - 2;
		free(code);
		return (1);
	}
	if ((*str)[var->i] == '{')
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
	if (!ft_isalpha((unsigned char)(*str)[var->i]) && (*str)[var->i] != '_')
		return (1);
	var->len = 0;
	while (ft_isalnum((unsigned char)(*str)[var->i]) || (*str)[var->i] == '_')
	{
		var->len++;
		var->i++;
	}
	return (0);
}

void	generate_strings_helper(char **str, char *expanded, t_variables *var)
{
	(void)var;
	*str = storing(*str, var->i - var->len - 1, var->len + 1, expanded);
	var->i += ft_strlen(expanded) - var->len - 1;
}

int	generate_string(char **str, char **tmp, t_variables *var, t_shell *pipe)
{
	char	*expanded;

	expanded = NULL;
	if (*tmp)
	{
		expanded = my_getenv(*tmp, pipe);
		if (expanded)
		{
			generate_strings_helper(str, expanded, var);
		}
		else if (!expanded)
		{
			*str = storing(*str, var->i - var->len - 1, var->len + 1, "");
			var->i -= var->len + 1;
		}
		free(*tmp);
		return (1);
	}
	return (0);
}

void	dollar_expansion(char **str, t_shell *pipe)
{
	t_variables	var;
	char		*tmp;
	char		*expanded;
	int			brace_syntax;
	int			var_start;

	var.len = 0;
	expanded = NULL;
	var.i = 0;
	var.in_d_quotes = 0;
	var.in_quotes = 0;
	while ((*str)[var.i])
	{
		quotes_check(str, &var);
		if ((*str)[var.i] == '$' && !var.in_quotes && (*str)[var.i + 1])
		{
			brace_syntax = 0;
			if ((*str)[var.i + 1] == '{')
				brace_syntax = 1;
			if (expand(str, expanded, &var))
				continue ;
			if (brace_syntax)
			{
				var_start = var.i - var.len + 1;
				tmp = ft_substr(*str, var_start, var.len - 2);
			}
			else
			{
				tmp = ft_substr(*str, var.i - var.len, var.len);
			}
			if (generate_string(str, &tmp, &var, pipe))
				continue ;
		}
		var.i++;
	}
}
