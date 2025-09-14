NAME = minishell
SRCS =	minishell.c parsing/signals.c  parsing/utils.c  parsing/parser.c parsing/redirect.c  parsing/spaces_parsing.c \
 parsing/env.c  free_all.c  parsing/pipes.c parsing/dollar.c parsing/cmds.c parsing/utils2.c \
 env_adapter.c \
 src/builtins/cd.c src/builtins/echo.c src/builtins/env_builtin.c src/builtins/exit_builtin.c \
 src/builtins/export.c src/builtins/export_helpers.c src/builtins/pwd.c src/builtins/unset.c \
 src/exec/child.c src/exec/dispatch.c src/exec/exec_conditions.c src/exec/exec_utils.c \
 src/exec/free.c src/exec/heredoc.c \
 src/exec/parent.c src/exec/parsed_exec.c src/exec/path.c src/exec/pipeline.c \
 src/exec/pipeline_helpers.c src/exec/pipeline_utils.c src/exec/redir.c src/exec/redir_utils.c \
 src/exec/status.c
OBJS = $(SRCS:.c=.o)
CC = cc

CFLAGS = -Wall -Wextra -Werror -g

all : $(NAME)

$(NAME): $(OBJS)
	make -C ./libft && make bonus -C ./libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L ./libft -lft -lreadline -I.brew/opt/readline/include -L/opt/vagrant/embedded/lib -lreadline

clean:
	cd libft && make fclean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
