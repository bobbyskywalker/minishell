CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell

SRCS = src/main.c \
       src/parser/AST_parser.c src/parser/AST_utils.c src/parser/parser_utils.c src/parser/lexer.c src/parser/tokenizer_utils.c \
       src/exec/cmd_validation.c src/exec/execute.c src/exec/exec_utils.c src/exec/heredoc.c src/exec/redirect.c src/exec/pipe.c \
       src/builtins/built_ins_export.c src/builtins/built_ins_unset.c src/builtins/built_ins_rest.c src/builtins/env_utils.c \
       src/signals/set_signals.c src/signals/reset_signals.c src/exec/redirect_utils.c src/exec/cleanup.c

OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT = lib/libft
LIBFT_LIB = $(LIBFT)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB) -lreadline

$(LIBFT_LIB):
	make -C $(LIBFT)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT_LIB)
	make -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re
