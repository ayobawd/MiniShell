/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_types.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exec integration <exec@integration.fix>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by exec              #+#    #+#             */
/*   Updated: 2025/01/17 00:00:00 by exec             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_TYPES_H
# define EXEC_TYPES_H

# include <stdbool.h>

/* Environment variable linked list */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* Redirection types for execution */
typedef enum e_redir_type
{
	R_IN,
	R_OUT_TRUNC,
	R_OUT_APPEND,
	R_HEREDOC
}	t_redir_type;

/* Redirection linked list */
typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	bool			quoted_delim;
	int				fd;
	struct s_redir	*next;
}	t_redir;

/* Command linked list for pipeline execution */
typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	bool			is_builtin;
	struct s_cmd	*next;
}	t_cmd;

/* Context for child process execution */
typedef struct s_child_context
{
	int	i;
	int	n;
	int	(*pipes)[2];
}	t_child_context;

#endif