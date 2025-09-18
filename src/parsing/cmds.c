/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:36:21 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/14 14:54:43 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**split_args_quoted(const char *s);

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

	start = i;
	qv.i = i;
	qv.in_quotes = 0;
	qv.in_d_quotes = 0;
	while (str[i])
	{
		qv.i = i;
		quotes_check(&str, &qv);
		if ((str[i] == ' ' || str[i] == '\t') && !qv.in_quotes
			&& !qv.in_d_quotes)
			break ;
		i++;
	}
	v->i = i;
	(*file_name) = ft_substr(str, start, i - start);
}

void	files_fillings(t_shell *pipe, t_cmds *cmds, t_variables *v)
{
	v->start = v->char_i;
	if (pipe->cmds[v->cmd_i][v->char_i + 1] == '>'
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
	while (pipe->cmds[v->cmd_i][++v->char_i])
	{
		v->i = v->char_i;
		quotes_check(&pipe->cmds[v->cmd_i], v);
		if ((pipe->cmds[v->cmd_i][v->char_i] == '>'
				|| pipe->cmds[v->cmd_i][v->char_i] == '<') && !v->quote_char)
		{
			files_fillings(pipe, cmds, v);
			store_the_file_name(pipe->cmds[v->cmd_i],
				&cmds[v->cmd_i].outs[v->xy].file_name, v->char_i + 1, v);
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
	*tmp = malloc(sizeof(t_cmds) * pipe->cmd_len);
	if (!*tmp)
		return ;
	cmds = *tmp;
	cmds->red_len = 0;
	while (++v.cmd_i < pipe->cmd_len)
	{
		if (!pipe->cmds || !pipe->cmds[v.cmd_i])
			break ;
		cmds[v.cmd_i].red_len = num_of_redirects(pipe->cmds[v.cmd_i]);
		if (cmds[v.cmd_i].red_len)
		{
			cmds[v.cmd_i].outs = malloc(sizeof(t_redirect)
					* cmds[v.cmd_i].red_len);
		}
		else
		{
			cmds[v.cmd_i].outs = NULL;
		}
		utils_saving(pipe, cmds, &v);
		cmds[v.cmd_i].cmds = split_args_quoted(pipe->cmds[v.cmd_i]);
		v.arg_i = 0;
		while (cmds[v.cmd_i].cmds[v.arg_i])
		{
			dollar_expansion(&cmds[v.cmd_i].cmds[v.arg_i], pipe);
			clean_quotes(cmds[v.cmd_i].cmds[v.arg_i++]);
		}
	}
}

static int	count_tokens_quoted(const char *s)
{
	int i;
	int count;
	int in_s;
	int in_d;

	i = 0;
	count = 0;
	in_s = 0;
	in_d = 0;
	while (s && s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;
		count++;
		while (s[i])
		{
			if (s[i] == '"' && !in_s)
				in_d = !in_d;
			else if (s[i] == '\'' && !in_d)
				in_s = !in_s;
			else if (s[i] == ' ' && !in_s && !in_d)
				break;
			i++;
		}
	}
	return (count);
}

static char	*substr_quoted(const char *s, int start, int end)
{
	return (ft_substr(s, start, end - start));
}

static char	**split_args_quoted(const char *s)
{
	int	 i;
	int	 start;
	int	 in_s;
	int	 in_d;
	int	 idx;
	int	 tokens;
	char **out;

	tokens = count_tokens_quoted(s);
	out = (char **)malloc(sizeof(char *) * (tokens + 1));
	if (!out)
		return (NULL);
	i = 0;
	idx = 0;
	in_s = 0;
	in_d = 0;
	while (s && s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;
		start = i;
		while (s[i])
		{
			if (s[i] == '"' && !in_s)
				in_d = !in_d;
			else if (s[i] == '\'' && !in_d)
				in_s = !in_s;
			else if (s[i] == ' ' && !in_s && !in_d)
				break;
			i++;
		}
		out[idx++] = substr_quoted(s, start, i);
	}
	out[idx] = NULL;
	return (out);
}
