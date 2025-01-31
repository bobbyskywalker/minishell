/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:13 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 10:52:20 by jzackiew         ###   ########.fr       */
/*   Updated: 2025/01/30 15:33:30 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
# include <signal.h>
#include "../lib/libft/libft.h"
#include "../lib/libft/gnl/get_next_line.h"
# include "parser.h"

// AST node type enumerations
typedef enum e_node_type
{
	COMMAND_NODE,
	PIPE_NODE,
	REDIRECT_NODE,
	// bonus (maybe) :))
	AND_NODE,
	OR_NODE
}						t_node_type;

typedef struct s_command
{
	char				**args;
}						t_command;

typedef struct s_ast_node
{
	t_node_type			type;
	struct s_ast_node	*left_child;
	struct s_ast_node	*right_child;
	t_command			*command;
}						t_ast_node;

char					**split_line(char *line);
void					execute_command(char *command);

// SECTION: lexer
char	**tokenize(char *source);

// SECTION: built-in commands
int	ft_echo(t_ast_node node);
int	ft_cd(t_ast_node node);
int ft_pwd(void);
int ft_exit(void);
int	ft_export(t_ast_node node);
int ft_unset(void);
int ft_env(void);

#endif
