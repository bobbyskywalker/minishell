CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell

SRCS = src/main.c \
       src/parser/AST_parser.c src/parser/AST_utils.c src/parser/parser_utils.c src/parser/lexer.c src/parser/tokenizer_utils.c \
	   src/exec/cmd_validation.c src/exec/execute.c src/exec/exec_utils.c \
	   src/builtins/built_ins_envset.c src/builtins/built_ins_rest.c src/builtins/env_utils.c 

OBJS = $(SRCS:.c=.o)

LIBFT = lib/libft
LIBFT_LIB = $(LIBFT)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB) -lreadline

$(LIBFT_LIB):
	make -C $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT_LIB)
	make -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re
