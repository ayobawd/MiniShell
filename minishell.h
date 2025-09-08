#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <limits.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

/* =====================
** Public Data Structures
** ===================== */

/* Exec-side env list */
typedef struct s_env
{
	char		*key;
	char		*val;
	struct s_env	*next;
} t_env;

/* Exec-side redirection types */
enum e_redir_type
{
	R_IN = 0,
	R_OUT_TRUNC,
	R_OUT_APPEND,
	R_HEREDOC
};

typedef struct s_redir
{
	int			 type;          /* enum e_redir_type */
	char		*target;        /* filename or heredoc delimiter */
	int			 fd;            /* for heredoc pipe, else -1 */
	bool		 quoted_delim;  /* heredoc delimiter quoted? */
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
	char		**argv;
	t_redir		*redirs;
	bool		 is_builtin;
	struct s_cmd	*next;
} t_cmd;

/* Parsing-side helpers (kept to reuse existing parser) */
extern int exit_code;
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
} t_redirect;

typedef struct s_cmds
{
	int			red_len;
	int			cmd_len;
	char		**cmds;
	t_redirect	*outs;
} t_cmds;

typedef struct s_varibles
{
	int	i;
	int	j;
	int	h;
	int	empty;
	int	space_found;
	int	quote_char;
	int	x;
	int	start;
	int	len;
	int	xy;
	int	in_d_quotes;
	int	in_quotes;
} t_variables;

typedef struct s_shell
{
	int		cmd_len;
	int		num_cmd;
	t_list	*environment; /* list of char* "KEY=VAL" for parser */
	int		environment_num;
	char	**cmds;        /* split pipeline segments */
} t_shell;

/* =====================
** Public API
** ===================== */

/* loop */
int		ms_loop(t_env **env);
char		*ms_prompt(t_env *env);
void		ms_signals_parent(void);
void		ms_signals_child_default(void);

/* env */
t_env		*ms_env_from_environ(char **environ);
char		*ms_env_get(t_env *env, const char *key);
int			ms_export(t_env **env, const char *key, const char *val);
int			ms_unset(t_env **env, const char *key);
void		ms_env_free(t_env *env);
char		**ms_env_to_envp(t_env *env);
int			ms_is_valid_key(const char *s);

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

/* New direct-parser execution interface */
int			ms_exec_parsed(t_cmds *arr, int count, t_env **env);
int			ms_exec_line_raw(char *line, t_env **env);

/* existing parsing functions (reused) */
char		**quote_aware_split(char *str);
void		files_saving(t_shell *pipe, t_cmds **tmp);
int			parsing(t_shell *pipe, t_cmds *cmds, char *input);
void		copy_env(t_shell *s, char **env);
char		*ft_add_spaces(char *input);
void		replace_spaces_tabs(char *str);
char		*my_getenv(const char *name, t_shell *pipe);
void		clean_quotes(char *str);
int			num_of_redirects(char *str);
void		remove_substr(char *s, unsigned int start, size_t len);
int			spaces(char *str);
int			is_spacee(int c);
int			redirections_parse(char *str);
int			handle_pipes(t_shell *pipe, char *input, t_cmds *cmds);
void		dollar_expansion(char **str, t_shell *pipe);

#endif
