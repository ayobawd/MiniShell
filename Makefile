NAME := minishell
CC := cc
CFLAGS := -Wall -Wextra -Werror

SRC_DIR := src
LIBFT_DIR := libft
INC := -I. -I$(LIBFT_DIR)
CFLAGS += $(INC)

SRCS := \
  $(SRC_DIR)/main.c \
  $(SRC_DIR)/loop/prompt.c \
  $(SRC_DIR)/loop/signals.c \
  $(SRC_DIR)/loop/line.c \
  $(SRC_DIR)/env/env.c \
  $(SRC_DIR)/env/env_utils.c \
  $(SRC_DIR)/env/env_convert.c \
  $(SRC_DIR)/env/utils.c \
  $(SRC_DIR)/builtins/echo.c \
  $(SRC_DIR)/builtins/cd.c \
  $(SRC_DIR)/builtins/pwd.c \
  $(SRC_DIR)/builtins/export.c \
  $(SRC_DIR)/builtins/unset.c \
  $(SRC_DIR)/builtins/env_builtin.c \
  $(SRC_DIR)/builtins/exit_builtin.c \
  $(SRC_DIR)/exec/dispatch.c \
  $(SRC_DIR)/exec/pipeline.c \
  $(SRC_DIR)/exec/redir.c \
  $(SRC_DIR)/exec/heredoc.c \
  $(SRC_DIR)/exec/path.c \
  $(SRC_DIR)/exec/child.c \
  $(SRC_DIR)/exec/parent.c \
  $(SRC_DIR)/exec/status.c \
  $(SRC_DIR)/exec/free.c \
  $(SRC_DIR)/exec/parsed_exec.c \
  $(SRC_DIR)/exec/line_raw.c \
  $(SRC_DIR)/parsing/ahmad_utils.c \
  $(SRC_DIR)/parsing/spaces_parsing.c \
  $(SRC_DIR)/parsing/redirect.c \
  $(SRC_DIR)/parsing/pipes.c \
  $(SRC_DIR)/parsing/parser.c \
  $(SRC_DIR)/parsing/env.c \
  $(SRC_DIR)/parsing/dollar.c \
  $(SRC_DIR)/parsing/dollar_utils.c \
  $(SRC_DIR)/parsing/dollar_helper.c \
  $(SRC_DIR)/parsing/cmds.c

OBJS := $(SRCS:.c=.o)

all: $(NAME)


$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) bonus
	$(CC) $(CFLAGS) $(INC) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
