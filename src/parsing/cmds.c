/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:36:21 by aradwan           #+#    #+#             */
/*   Updated: 2025/09/05 17:41:54 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	store_the_file_name(char *str, char **file_name, int i, t_variables *var)
{
	int	start;
	int	in_quote;

	start = i;
	in_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!in_quote)
				in_quote = str[i];
			else if (in_quote == str[i])
				in_quote = 0;
		}
		if ((str[i] == ' ' || str[i] == '\t') && !in_quote)
			break ;
		i++;
	}
	var->i = i;
	(*file_name) = ft_substr(str, start, i - start);
}

void	files_fillings(t_shell *pipe, t_cmds *cmds, t_variables *var)
{
	var->start = var->x - 1;
	if (pipe->cmds[var->j][var->x + 1] == '>' \
		|| pipe->cmds[var->j][var->x + 1] == '<')
	{
		if (pipe->cmds[var->j][var->x + 1] == '>')
			cmds[var->j].outs[var->xy].flag = APPEND;
		else if (pipe->cmds[var->j][var->x + 1] == '<')
			cmds[var->j].outs[var->xy].flag = HERE_DOC;
		var->x = var->x + 2;
	}
	else if (pipe->cmds[var->j][var->x] == '>')
	{
		cmds[var->j].outs[var->xy].flag = OUT_FILE;
		var->x++;
	}
	else if (pipe->cmds[var->j][var->x] == '<')
	{
		cmds[var->j].outs[var->xy].flag = IN_FILE;
		var->x++;
	}
}

void	utils_saving(t_shell *pipe, t_cmds *cmds, t_variables *v)
{
	v->xy = 0;
	v->x = -1;
	while (pipe->cmds[v->j][++v->x])
	{
		if (pipe->cmds[v->j][v->x] == '"' || pipe->cmds[v->j][v->x] == '\'')
		{
			if (v->quote_char == 0)
				v->quote_char = pipe->cmds[v->j][v->x];
			else if (v->quote_char == pipe->cmds[v->j][v->x])
				v->quote_char = 0;
		}
		if ((pipe->cmds[v->j][v->x] == '>' || \
			pipe->cmds[v->j][v->x] == '<') && !v->quote_char)
		{
			files_fillings(pipe, cmds, v);
			store_the_file_name(pipe->cmds[v->j], \
				&cmds[v->j].outs[v->xy].file_name, v->x + 1, v);
			clean_quotes(cmds[v->j].outs[v->xy].file_name);
			remove_substr(pipe->cmds[v->j], v->start, v->i);
			v->x = v->start - 1;
			v->xy++;
		}
	}
}

char	**quote_aware_split(char *str)
{
	char	**result;
	int		i, j, start, tokens;
	int		in_quote;
	char	quote_char;

	if (!str)
		return (NULL);
	
	/* Count tokens first */
	tokens = 0;
	i = 0;
	in_quote = 0;
	quote_char = 0;
	
	while (str[i])
	{
		/* Skip leading spaces */
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break;
		
		/* Count this token */
		tokens++;
		
		/* Skip to end of token */
		while (str[i])
		{
			if (!in_quote && (str[i] == '"' || str[i] == '\''))
			{
				in_quote = 1;
				quote_char = str[i];
			}
			else if (in_quote && str[i] == quote_char)
			{
				in_quote = 0;
				quote_char = 0;
			}
			else if (!in_quote && (str[i] == ' ' || str[i] == '\t'))
				break;
			i++;
		}
	}
	
	/* Allocate result array */
	result = malloc(sizeof(char *) * (tokens + 1));
	if (!result)
		return (NULL);
	
	/* Extract tokens */
	i = 0;
	j = 0;
	in_quote = 0;
	quote_char = 0;
	
	while (str[i] && j < tokens)
	{
		/* Skip leading spaces */
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break;
			
		start = i;
		
		/* Find end of token */
		while (str[i])
		{
			if (!in_quote && (str[i] == '"' || str[i] == '\''))
			{
				in_quote = 1;
				quote_char = str[i];
			}
			else if (in_quote && str[i] == quote_char)
			{
				in_quote = 0;
				quote_char = 0;
			}
			else if (!in_quote && (str[i] == ' ' || str[i] == '\t'))
				break;
			i++;
		}
		
		/* Extract token */
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

void	files_saving(t_shell *pipe, t_cmds **tmp)
{
	t_cmds	*cmds;
	t_variables	var;

	var.start = 0;
	var.quote_char = 0;
	var.h = 0;
	var.j = -1;
	var.x = 0;
	*tmp = malloc(sizeof(t_cmds) * pipe->cmd_len);
	cmds = *tmp;
	cmds->red_len = 0; //     echo hi > 'file1   khk' < file2 | cat -e | ls -la
	while (++var.j < pipe->cmd_len)
	{
		cmds[var.j].red_len = num_of_redirects(pipe->cmds[var.j]);
		if (cmds[var.j].red_len)
			cmds[var.j].outs = malloc(sizeof(t_redirect) * cmds[var.j].red_len);
		utils_saving(pipe, cmds, &var);
		cmds[var.j].cmds = quote_aware_split(pipe->cmds[var.j]);
		var.h = 0;
		while (cmds[var.j].cmds[var.h])
			clean_quotes(cmds[var.j].cmds[var.h++]);
		var.h = 0;
		while (cmds[var.j].cmds[var.h])
			var.h++;
	}
}

