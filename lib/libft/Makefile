NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

SOURCES = \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_isspace.c\
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_atoi.c \
	ft_calloc.c ft_strdup.c ft_strnstr.c ft_substr.c ft_strtrim.c \
	ft_split.c ft_strjoin.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_print_str.c ft_print_nums.c ft_print_hex.c ft_print_ptr.c ft_printf.c \
	gnl/get_next_line.c gnl/get_next_line_utils.c ft_arr2d_free.c ft_slc_sort.c \
	ft_realloc.c ft_2d_strdup.c ft_2d_strlen.c ft_2d_strprintf.c

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs -o $(NAME) $(OBJECTS)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re