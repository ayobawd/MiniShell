/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	print_export_env(t_shell *shell)
{
	t_list	*current;
	char	*equal_pos;
	char	*key;
	char	*value;

	current = shell->environment;
	while (current)
	{
		equal_pos = ft_strchr((char *)current->content, '=');
		if (equal_pos)
		{
			key = ft_substr((char *)current->content, 0, equal_pos
					- (char *)current->content);
			value = ft_strdup(equal_pos + 1);
			printf("declare -x %s=\"%s\"\n", key, value);
			free(key);
			free(value);
		}
		else
			printf("declare -x %s\n", (char *)current->content);
		current = current->next;
	}
	return (0);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
