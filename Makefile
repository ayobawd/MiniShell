NAME = minishell
SRCS =	minishell.c src/parsing/signals.c  src/parsing/utils.c  src/parsing/parser.c src/parsing/redirect.c  src/parsing/spaces_parsing.c \
 src/parsing/env.c  free_all.c  src/parsing/pipes.c src/parsing/dollar.c src/parsing/cmds.c src/parsing/utils2.c \
 src/builtins/echo.c src/builtins/cd.c src/builtins/pwd.c src/builtins/export.c src/builtins/export_helpers.c \
 src/builtins/unset.c src/builtins/env_builtin.c src/builtins/exit_builtin.c \
 src/exec/dispatch.c src/exec/pipeline.c src/exec/pipeline_helpers.c src/exec/pipeline_utils.c \
 src/exec/redir.c src/exec/redir_utils.c src/exec/heredoc.c src/exec/status.c \
 src/exec/path.c src/exec/free.c src/exec/env_utils.c src/exec/signals.c src/exec/adapter.c \
 src/exec/exec_utils.c src/exec/exec_conditions.c
OBJS = $(SRCS:.c=.o)
CC = cc

CFLAGS = -Wall -Wextra -Werror -g -I.

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
