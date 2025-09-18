NAME = minishell

PARSING_DIR = src/parsing
EXECUTION_DIR = src/execution

SRCS = \
	minishell.c \
	free_all.c \
	$(PARSING_DIR)/signals.c \
	$(PARSING_DIR)/utils.c \
	$(PARSING_DIR)/parser.c \
	$(PARSING_DIR)/redirect.c \
	$(PARSING_DIR)/spaces_parsing.c \
	$(PARSING_DIR)/env.c \
	$(PARSING_DIR)/pipes.c \
	$(PARSING_DIR)/dollar.c \
	$(PARSING_DIR)/cmds.c \
	$(PARSING_DIR)/utils2.c \
	$(EXECUTION_DIR)/execution.c \
	$(EXECUTION_DIR)/execution_utils.c \
	$(EXECUTION_DIR)/builtins.c \
	$(EXECUTION_DIR)/builtins_basic.c \
	$(EXECUTION_DIR)/builtins_env.c \
	$(EXECUTION_DIR)/builtins_exit.c \
	$(EXECUTION_DIR)/builtins_unset.c \
	$(EXECUTION_DIR)/env_utils.c \
	$(EXECUTION_DIR)/env_helpers.c \
	$(EXECUTION_DIR)/redirections.c \
	$(EXECUTION_DIR)/redirection_utils.c \
	$(EXECUTION_DIR)/pipelines.c \
	$(EXECUTION_DIR)/pipelines_utils.c \
	$(EXECUTION_DIR)/pipelines_exec.c

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I.

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C ./libft && $(MAKE) bonus -C ./libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L ./libft -lft -lreadline

clean:
	$(MAKE) -C libft fclean
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
