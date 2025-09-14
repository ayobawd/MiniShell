NAME = minishell
SRCS =	minishell.c src/parsing/signals.c  src/parsing/utils.c  src/parsing/parser.c src/parsing/redirect.c  src/parsing/spaces_parsing.c \
 src/parsing/env.c  free_all.c  src/parsing/pipes.c src/parsing/dollar.c src/parsing/cmds.c src/parsing/utils2.c \
 src/builtins/echo.c src/builtins/pwd.c simple_exec.c
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
