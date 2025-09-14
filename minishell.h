/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: aradwan <aradwan@student.42.fr>            +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/06/24 16:18:39 by aradwan           #+#    #+#             */
/*   Updated: 2025/08/15 20:16:07 by aradwan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

extern int	g_exit_code;

enum e_types
{
	IN_FILE,
	OUT_FILE,
	APPEND,
	HERE_DOC,
};

typedef struct s_redirect
{
	int		flag;
	char	*file_name;
}	t_redirect;

typedef struct s_cmds
{
	int			red_len;
	int			cmd_len;
	char		**cmds;
	t_redirect	*outs;
}	t_cmds;

typedef struct s_variables
{
	int	i;
	int	indx;
	int	cmd_i;
	int	arg_i;
	int	space_found;
	int	quote_char;
	int	char_i;
	int	start;
	int	len;
	int	xy;
	int	in_d_quotes;
	int	in_quotes;
}	t_variables;

typedef struct s_shell
{
	int		cmd_len;
	int		num_cmd;
	t_list	*environment;
	int		environment_num;
	char	**cmds;
}	t_shell;

/* Environment variable structure for exec system */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* Redirection types for exec system */
typedef enum e_redir_type
{
	R_IN,
	R_OUT_TRUNC,
	R_OUT_APPEND,
	R_HEREDOC
}	t_redir_type;

/* Redirection structure for exec system */
typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	bool				quoted_delim;
	int					fd;
	struct s_redir		*next;
}	t_redir;

/* Command structure for exec system */
typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* Child execution context */
typedef struct s_child_context
{
	int		i;
	int		n;
	int		(*pipes)[2];
}	t_child_context;

//           environment              \\.
int		parsing(t_shell *pipe, t_cmds *cmds, char *input);
void	copy_env(t_shell *s, char **env);
char	*ft_add_spaces(char *input);
void	replace_spaces_tabs(char *str);
char	*my_getenv(const char *name, t_shell *pipe);
void	clean_quotes(char *str);

//            utils             \\.
int		spaces(char *str);
int		is_spacee(int c);
void	free_all(t_shell *pipe, t_cmds *cmd);
void	free_strings(char **av);
void	remove_substr(char *s, unsigned int start, size_t len);

//            signals            \\.
void	handle_signals(int signal);

//            redirect           \\.
int		redirections_parse(char *str);
int		num_of_redirects(char *str);

//             pipes             \\.
int		handle_pipes(t_shell *pipe, char *input, t_cmds *cmds);

//             cmds              \\.
void	init_commands(t_shell *pipe, t_cmds **tmp);

void	rl_replace_line(const char *text, int clear_undo);
void	rl_redisplay(void);
void	quotes_check(char **str, t_variables *v);

/* builtins */
int			bi_echo(char **argv);
int			bi_cd(char **argv, t_env **env);
int			bi_pwd(char **argv);
int			bi_export(char **argv, t_env **env);
int			bi_unset(char **argv, t_env **env);
int			bi_env(char **argv, t_env **env);
int			bi_exit(char **argv, t_env **env, bool in_parent);

/* exec */
int			ms_execute_line(t_cmd *pipeline, t_env **env);
int			ms_exec_pipeline(t_cmd *first, t_env **env);
int			ms_apply_redirs(t_redir *rlist, int fds[2]);
int			ms_run_heredoc(const char *delim, bool quoted, int *out_fd);
char		*ms_resolve_path(const char *cmd, t_env *env);
int			ms_status_get(void);
void		ms_status_set(int st);
void		ms_cmd_free(t_cmd *pipeline);
int			ms_is_builtin(const char *name);
int			ms_run_builtin(char **argv, t_env **env, bool in_parent);



#endif