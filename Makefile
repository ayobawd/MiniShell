NAME = minishell
SRCS =	minishell.c parsing/signals.c  parsing/utils.c  parsing/parser.c parsing/redirect.c  parsing/spaces_parsing.c \
 parsing/env.c  free_all.c  parsing/pipes.c parsing/dollar.c parsing/cmds.c parsing/utils2.c
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
