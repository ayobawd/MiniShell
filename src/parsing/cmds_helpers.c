/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 <student@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 00:00:00 by 42                #+#    #+#             */
/*   Updated: 2025/09/05 00:00:00 by 42               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	skip_token(char *str, int *i, int *in_quote, char *quote_char)
{
	while (str[*i] && ((*in_quote && str[*i] != *quote_char)
			|| (!*in_quote && str[*i] != ' ' && str[*i] != '\t')))
	{
		if (!*in_quote && (str[*i] == '"' || str[*i] == '\''))
		{
			*in_quote = 1;
			*quote_char = str[*i];
		}
		else if (*in_quote && str[*i] == *quote_char)
		{
			*in_quote = 0;
			*quote_char = 0;
		}
		(*i)++;
	}
}

int	count_tokens(char *str)
{
	int		i;
	int		tokens;
	int		in_quote;
	char	quote_char;

	tokens = 0;
	i = 0;
	in_quote = 0;
	quote_char = 0;
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break ;
		tokens++;
		skip_token(str, &i, &in_quote, &quote_char);
	}
	return (tokens);
}

static void	extract_single_token(char *str, int *i, t_variables *v)
{
	while (str[*i])
	{
		if (!v->in_quotes && (str[*i] == '"' || str[*i] == '\''))
		{
			v->in_quotes = 1;
			v->quote_char = str[*i];
		}
		else if (v->in_quotes && str[*i] == v->quote_char)
		{
			v->in_quotes = 0;
			v->quote_char = 0;
		}
		else if (!v->in_quotes && (str[*i] == ' ' || str[*i] == '\t'))
			break ;
		(*i)++;
	}
}

char	**extract_tokens(char *str, int tokens)
{
	char		**result;
	int			i;
	int			j;
	int			start;
	t_variables	v;

	result = malloc(sizeof(char *) * (tokens + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	v.in_quotes = 0;
	v.quote_char = 0;
	while (str[i] && j < tokens)
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break ;
		start = i;
		extract_single_token(str, &i, &v);
		result[j] = ft_substr(str, start, i - start);
		if (!result[j])
		{
			while (j > 0)
				free(result[--j]);
			free(result);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}

void	init_files_saving_vars(t_variables *var)
{
	var->start = 0;
	var->quote_char = 0;
	var->h = 0;
	var->j = -1;
	var->x = 0;
}

void	process_cmd_segment(t_shell *pipe, t_cmds *cmds, t_variables *var)
{
	cmds[var->j].red_len = num_of_redirects(pipe->cmds[var->j]);
	if (cmds[var->j].red_len)
		cmds[var->j].outs = malloc(sizeof(t_redirect) * cmds[var->j].red_len);
	utils_saving(pipe, cmds, var);
	cmds[var->j].cmds = quote_aware_split(pipe->cmds[var->j]);
	var->h = 0;
	while (cmds[var->j].cmds[var->h])
		clean_quotes(cmds[var->j].cmds[var->h++]);
	var->h = 0;
	while (cmds[var->j].cmds[var->h])
		var->h++;
}
