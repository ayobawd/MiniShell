/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/XX XX:XX:XX by aradwan           #+#    #+#             */
/*   Updated: 2025/01/XX XX:XX:XX by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**create_env_array(t_shell *shell)
{
	char	**env_array;
	t_list	*current;
	int		count;
	int		i;

	count = 0;
	current = shell->environment;
	while (current && ++count)
		current = current->next;
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = shell->environment;
	while (current)
	{
		env_array[i] = ft_strdup((char *)current->content);
		if (!env_array[i])
			return (free_strings(env_array), NULL);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

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
			key = ft_substr((char *)current->content, 0,
					equal_pos - (char *)current->content);
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

int	set_env_var(t_shell *shell, char *key, char *value)
{
	char	*new_var;
	char	*temp;
	t_list	*current;
	t_list	*new_node;

	if (!key || !value)
		return (1);
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (1);
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return (1);
	current = shell->environment;
	while (current && (ft_strncmp((char *)current->content, key,
				ft_strlen(key)) != 0
			|| ((char *)current->content)[ft_strlen(key)] != '='))
		current = current->next;
	if (current)
		return (free(current->content), current->content = new_var, 0);
	new_node = ft_lstnew(new_var);
	if (!new_node)
		return (free(new_var), 1);
	ft_lstadd_back(&shell->environment, new_node);
	return (0);
}

int	unset_env_var(t_shell *shell, char *key)
{
	t_list	*current;
	t_list	*prev;
	size_t	key_len;

	if (!key)
		return (1);
	current = shell->environment;
	prev = NULL;
	key_len = ft_strlen(key);
	while (current && (ft_strncmp((char *)current->content, key, key_len) != 0
			|| (((char *)current->content)[key_len] != '='
			&& ((char *)current->content)[key_len] != '\0')))
	{
		prev = current;
		current = current->next;
	}
	if (!current)
		return (0);
	if (prev)
		prev->next = current->next;
	else
		shell->environment = current->next;
	free(current->content);
	free(current);
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
