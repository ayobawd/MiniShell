/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	*create_declare_line_with_val(t_env *cur)
{
	char	*prefix;
	char	*eq;
	char	*val;
	char	*line;

	prefix = ft_strjoin("declare -x ", cur->key);
	eq = ft_strjoin(prefix, "=\"");
	val = ft_strjoin(eq, cur->val);
	line = ft_strjoin(val, "\"\n");
	free(prefix);
	free(eq);
	free(val);
	return (line);
}

char	*create_declare_line_no_val(t_env *cur)
{
	char	*prefix;
	char	*line;

	prefix = ft_strjoin("declare -x ", cur->key);
	line = ft_strjoin(prefix, "\n");
	free(prefix);
	return (line);
}

void	sort_export_rows(char **rows, int n)
{
	int		i;
	int		swapped;
	char	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 1;
		while (i < n)
		{
			if (ft_strncmp(rows[i - 1], rows[i],
					ft_strlen(rows[i - 1]) + 1) > 0)
			{
				tmp = rows[i - 1];
				rows[i - 1] = rows[i];
				rows[i] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

void	print_export_err(const char *prefix, const char *name, const char *msg)
{
	write(2, prefix, ft_strlen(prefix));
	write(2, name, ft_strlen(name));
	write(2, msg, ft_strlen(msg));
}
