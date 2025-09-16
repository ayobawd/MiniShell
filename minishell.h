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

/* Removed non-parsing exec related structures per cleanup */

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




#endif