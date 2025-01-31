/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:13 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 14:22:13 by agarbacz         ###   ########.fr       */
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

char					**split_line(char *line);
void					execute_command(char *command);

// SECTION: built-in commands
int						ft_echo(t_ast_node node);
int						ft_cd(t_ast_node node);
int						ft_pwd(void);
int						ft_exit(void);
int						ft_export(t_ast_node node);
int						ft_unset(void);
int						ft_env(void);

#endif
