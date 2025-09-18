/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:30:38 by ayal-awa          #+#    #+#             */
/*   Updated: 2025/09/16 11:30:38 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_space_tab(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_sign(char c)
{
	return (c == '+' || c == '-');
}

int	parse_exit_number(const char *s, int *out)
{
	int	i;
	int	has_digit;

	i = 0;
	while (s[i] && is_space_tab(s[i]))
		i++;
	if (s[i] && is_sign(s[i]))
		i++;
	has_digit = (s[i] && ft_isdigit(s[i]));
	if (!has_digit)
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (s[i] != '\0')
		return (0);
	*out = ft_atoi(s);
	return (1);
}

int	print_ident_err(const char *cmd, const char *arg)
{
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}
