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
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
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

/* Removed non-parsing exec related structures per cleanup */

//           environment              \\.
int		parsing(t_shell *pipe, t_cmds *cmds, char *input);
void	copy_env(t_shell *s, char **env);
char	*ft_add_spaces(char *input);
void	replace_spaces_tabs(char *str);
char	*my_getenv(const char *name, t_shell *pipe);
void	clean_quotes(char *str);
void	dollar_expansion(char **str, t_shell *pipe);

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

//           execution           \\.
int		execute_commands(t_shell *shell, t_cmds *cmds);
int		execute_single_command(t_shell *shell, t_cmds *cmd);
int		execute_external_command(t_shell *shell, t_cmds *cmd);
int		is_builtin(char *cmd);
char	*find_command_path(char *cmd, t_shell *shell);

//          redirections        \\.
int		setup_redirections(t_cmds *cmd);
int		setup_heredoc(char *delimiter);
int		restore_std_fds(void);

//          pipelines           \\.
int		execute_pipeline(t_shell *shell, t_cmds *cmds, int cmd_count);
int		execute_single_command_with_redirections(t_shell *shell, t_cmds *cmd);
int		**create_pipes(int num_pipes);
void	setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count);
void	close_all_pipes(int **pipes, int num_pipes);
void	free_pipes(int **pipes, int num_pipes);
int		wait_for_children(pid_t *pids, int count);
int		should_fork_builtin(t_cmds *cmd);
void	restore_fds(int saved_stdin, int saved_stdout);

//           builtins            \\.
int		execute_builtin(t_shell *shell, t_cmds *cmd);
int		builtin_echo(t_cmds *cmd);
int		builtin_cd(t_shell *shell, t_cmds *cmd);
int		builtin_pwd(void);
int		builtin_env(t_shell *shell);
int		builtin_export(t_shell *shell, t_cmds *cmd);
int		builtin_unset(t_shell *shell, t_cmds *cmd);
int		builtin_exit(t_cmds *cmd);

//          env_utils           \\.
char	**create_env_array(t_shell *shell);
int		print_export_env(t_shell *shell);
int		set_env_var(t_shell *shell, char *key, char *value);
int		unset_env_var(t_shell *shell, char *key);
int		is_valid_identifier(char *str);

void	rl_replace_line(const char *text, int clear_undo);
void	rl_redisplay(void);
void	quotes_check(char **str, t_variables *v);




#endif