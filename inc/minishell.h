#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../lib/libft/libft.h"
#include "../lib/libft/gnl/get_next_line.h"

typedef struct s_token
{
	char **cmd;
    char *file_out;
    char *file_in;
} t_token;


char **split_line(char *line);


#endif
