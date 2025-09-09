/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
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
	(void)str;
	(void)var;
	/* Temporarily disable quote tracking to debug hang */
}

static void	generate_strings_helper(char **str, char *expanded,
		t_variables *var)
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
