/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:13 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/05 18:53:35 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/libft.h"
# include "parser.h"
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_shell_data
{
	char	**env_vars;
	int		last_cmd_status;
}				t_shell_data;


char					**split_line(char *line);
void					execute_command(char *command);

// SECTION: lexer
char					**tokenize(char *source);

// SECTION: built-in commands
int						ft_echo(t_ast_node node);
int						ft_cd(t_ast_node node, t_shell_data shell_data);
int						ft_pwd(void);
int						ft_exit(void);
int						ft_export(t_ast_node node, t_shell_data *shell_data);
int						ft_unset(t_ast_node node, t_shell_data *shell_data);
int						ft_env(t_shell_data shell_data);

#endif
