/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-awa <ayal-awa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:36:21 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/21 17:05:16 by ayal-awa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int count_args_outside_quotes(char *input)
{
	int i;
	int count;
	int in_quote;
	int in_dquote;
	int in_arg;

	i = 0;
	count = 0;
	in_quote = 0;
	in_dquote = 0;
	in_arg = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (input[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if ((input[i] == ' ' || input[i] == '\t') && !in_quote && !in_dquote)
		{
			if (in_arg)
			{
				in_arg = 0;
			}
		}
		else if (!in_arg)
		{
			in_arg = 1;
			count++;
		}
		i++;
	}
	return (count);
}

static char **split_args_respect_quotes(char *input)
{
	char **result;
	int arg_count;
	int i, start, arg_idx;
	int in_quote, in_dquote, in_arg;

	arg_count = count_args_outside_quotes(input);
	result = malloc(sizeof(char *) * (arg_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	arg_idx = 0;
	in_quote = 0;
	in_dquote = 0;
	in_arg = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (input[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		
		if ((input[i] == ' ' || input[i] == '\t') && !in_quote && !in_dquote)
		{
			if (in_arg)
			{
				result[arg_idx] = ft_substr(input, start, i - start);
				if (!result[arg_idx])
					return (free_strings(result), NULL);
				arg_idx++;
				in_arg = 0;
			}
		}
		else if (!in_arg)
		{
			start = i;
			in_arg = 1;
		}
		i++;
	}
	if (in_arg)
	{
		result[arg_idx] = ft_substr(input, start, i - start);
		if (!result[arg_idx])
			return (free_strings(result), NULL);
		arg_idx++;
	}
	result[arg_idx] = NULL;
	return (result);
}

void	remove_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (i < start || i >= len)
		{
			s[j] = s[i];
			j++;
		}
		i++;
	}
	s[j] = '\0';
}

void	store_the_file_name(char *str, char **file_name, int i, t_variables *v)
{
	int			start;
	t_variables	qv;

	// Skip leading spaces
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	start = i;
	qv.i = i;
	qv.in_quotes = 0;
	qv.in_d_quotes = 0;
	while (str[i])
	{
		qv.i = i;
		quotes_check(&str, &qv);
		if (((str[i] == ' ' || str[i] == '\t' || str[i] == '>' || str[i] == '<') && \
!qv.in_quotes && !qv.in_d_quotes))
			break ;
		i++;
	}
	v->i = i;
	(*file_name) = ft_substr(str, start, i - start);
	if (!*file_name)
		return ;
}

void	files_fillings(t_shell *pipe, t_cmds *cmds, t_variables *v)
{
	v->start = v->char_i - 1;
	if (pipe->cmds[v->cmd_i][v->char_i + 1] == '>' \
|| pipe->cmds[v->cmd_i][v->char_i + 1] == '<')
	{
		if (pipe->cmds[v->cmd_i][v->char_i + 1] == '>')
			cmds[v->cmd_i].outs[v->xy].flag = APPEND;
		else if (pipe->cmds[v->cmd_i][v->char_i + 1] == '<')
			cmds[v->cmd_i].outs[v->xy].flag = HERE_DOC;
		v->char_i = v->char_i + 2;
	}
	else if (pipe->cmds[v->cmd_i][v->char_i] == '>')
	{
		cmds[v->cmd_i].outs[v->xy].flag = OUT_FILE;
		v->char_i++;
	}
	else if (pipe->cmds[v->cmd_i][v->char_i] == '<')
	{
		cmds[v->cmd_i].outs[v->xy].flag = IN_FILE;
		v->char_i++;
	}
}

void	utils_saving(t_shell *pipe, t_cmds *cmds, t_variables *v)
{
	v->xy = 0;
	v->char_i = -1;
	v->in_quotes = 0;
	v->in_d_quotes = 0;
	while (pipe->cmds[v->cmd_i][++v->char_i])
	{
		v->i = v->char_i;
		quotes_check(&pipe->cmds[v->cmd_i], v);
		if ((pipe->cmds[v->cmd_i][v->char_i] == '>' || \
pipe->cmds[v->cmd_i][v->char_i] == '<') && !v->in_quotes && !v->in_d_quotes)
		{
			files_fillings(pipe, cmds, v);
			store_the_file_name(pipe->cmds[v->cmd_i], \
&cmds[v->cmd_i].outs[v->xy].file_name, v->char_i, v);
			clean_quotes(cmds[v->cmd_i].outs[v->xy].file_name);
			remove_substr(pipe->cmds[v->cmd_i], v->start, v->i);
			v->char_i = v->start - 1;
			v->xy++;
		}
	}
}

void	init_commands(t_shell *pipe, t_cmds **tmp)
{
	t_cmds		*cmds;
	t_variables	v;

	v.start = 0;
	v.quote_char = 0;
	v.arg_i = 0;
	v.cmd_i = -1;
	v.char_i = 0;
	v.in_quotes = 0;
	v.in_d_quotes = 0;
	*tmp = malloc(sizeof(t_cmds) * pipe->cmd_len);
	if (!*tmp)
		return ;
	cmds = *tmp;
	while (++v.cmd_i < pipe->cmd_len)
	{
		cmds[v.cmd_i].red_len = num_of_redirects(pipe->cmds[v.cmd_i]);
		cmds[v.cmd_i].outs = NULL;
		if (cmds[v.cmd_i].red_len)
		{
			cmds[v.cmd_i].outs = malloc(sizeof(t_redirect) * \
cmds[v.cmd_i].red_len);
			if (!cmds[v.cmd_i].outs)
				return ;
		}
		utils_saving(pipe, cmds, &v);
		cmds[v.cmd_i].cmds = split_args_respect_quotes(pipe->cmds[v.cmd_i]);
		v.arg_i = 0;
		while (cmds[v.cmd_i].cmds[v.arg_i])
			clean_quotes(cmds[v.cmd_i].cmds[v.arg_i++]);
	}
}
