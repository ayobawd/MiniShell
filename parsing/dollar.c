/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:23:19 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:53:19 by aradwan          ###   ########.fr       */
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
	new_str = malloc(sizeof(char) * \
(ft_strlen(str) - len + ft_strlen(replace) + 1));
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

int	expand(char **str, char *expanded, t_variables *v)
{
	expanded = NULL;
	v->i++;
	if ((*str)[v->i] == '?' && v->i == 1 && (*str)[v->i - 1] == '$')
	{
		expanded = ft_itoa(g_exit_code);
		*str = storing(*str, v->i - 1, 2, expanded);
		v->i += ft_strlen(expanded) - 2;
		free(expanded);
		return (1);
	}
	if (!ft_isalpha((*str)[v->i]) && (*str)[v->i] != '_')
		return (1);
	v->len = 0;
	while (ft_isalnum((*str)[v->i]) || (*str)[v->i] == '_')
	{
		v->len++;
		v->i++;
	}
	return (0);
}

void	generate_strings_helper(char **str, char *expanded, t_variables *v)
{
	char	*joined;
	char	*joined2;

	if (!v->in_d_quotes)
	{
		joined = ft_strjoin(expanded, "\"");
		joined2 = ft_strjoin("\"", joined);
		*str = storing(*str, v->i - v->len - 1, v->len + 1, joined2);
		v->i += ft_strlen(expanded) - v->len - 1;
		free(joined2);
		free(joined);
	}
	else
	{
		*str = storing(*str, v->i - v->len - 1, v->len + 1, expanded);
		v->i += ft_strlen(expanded) - v->len - 1;
	}
}

int	generate_string(char **str, char **tmp, t_variables *v, t_shell *pipe)
{
	char	*expanded;

	expanded = NULL;
	if (*tmp)
	{
		expanded = my_getenv(*tmp, pipe);
		if (expanded)
		{
			generate_strings_helper(str, expanded, v);
		}
		else if (!expanded)
		{
			*str = storing(*str, v->i - v->len - 1, v->len + 1, "");
			v->i -= v->len + 1;
		}
		free(*tmp);
		return (1);
	}
	return (0);
}

void	dollar_expansion(char **str, t_shell *pipe)
{
	t_variables	v;
	char		*tmp;
	char		*expanded;

	v.len = 0;
	expanded = NULL;
	v.i = 0;
	v.in_d_quotes = 0;
	v.in_quotes = 0;
	while ((*str)[v.i])
	{
		quotes_check(str, &v);
		if ((*str)[v.i] == '$' && !v.in_quotes && (*str)[v.i + 1])
		{
			if (expand(str, expanded, &v))
				continue ;
			tmp = ft_substr(*str, v.i - v.len, v.len);
			if (generate_string(str, &tmp, &v, pipe))
				continue ;
		}
		v.i++;
	}
}
