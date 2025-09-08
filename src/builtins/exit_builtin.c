/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int ms_status_get(void);

static int  is_space(int c) { return (c == ' ' || (c >= '\t' && c <= '\r')); }

static int  parse_ll(const char *s, long long *out)
{
	int         sign;
	unsigned long long val;

	while (is_space((unsigned char)*s)) s++;
	sign = 1;
	if (*s == '+' || *s == '-') { if (*s == '-') sign = -1; s++; }
	if (!*s) return (0);
	val = 0;
	while (*s)
	{
	    if (*s < '0' || *s > '9') return (0);
	    if (val > (unsigned long long)LLONG_MAX / 10ULL)
	        return (0);
	    val = val * 10ULL + (unsigned long long)(*s - '0');
	    if (sign > 0 && val > (unsigned long long)LLONG_MAX)
	        return (0);
	    if (sign < 0 && val > (unsigned long long)LLONG_MAX + 1ULL)
	        return (0);
	    s++;
	}
	if (sign > 0)
	    *out = (long long)val;
	else
	{
	    if (val == (unsigned long long)LLONG_MAX + 1ULL)
	        *out = LLONG_MIN;
	    else
	        *out = -(long long)val;
	}
	return (1);
}

static void print_err(const char *a, const char *b, const char *c)
{
	write(2, a, ft_strlen(a));
	write(2, b, ft_strlen(b));
	write(2, c, ft_strlen(c));
}

int bi_exit(char **argv, t_env **env, bool in_parent)
{
	long long   code_ll;
	int         ac;
	int         status;

	(void)env;
	ac = 0; while (argv[ac]) ac++;
	write(1, "exit\n", 5);
	if (ac == 1)
	{
	    status = ms_status_get();
	    if (in_parent)
	        exit(status);
	    return (status);
	}
	if (!parse_ll(argv[1], &code_ll))
	{
	    print_err("exit: ", argv[1], ": numeric argument required\n");
	    if (in_parent)
	        exit(2);
	    return (2);
	}
	if (ac > 2)
	{
	    print_err("exit: ", "", "too many arguments\n");
	    return (1);
	}
	status = (int)((unsigned char)(code_ll & 0xFF));
	if (in_parent)
	    exit(status);
	return (status);
}
