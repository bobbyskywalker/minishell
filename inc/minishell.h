/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:13 by agarbacz          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/07 17:56:54 by agarbacz         ###   ########.fr       */
=======
/*   Updated: 2025/02/05 18:53:35 by jzackiew         ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/libft.h"
# include "parser.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell_data
{
	char	**env_vars;
	int		last_cmd_status;
}			t_shell_data;

char		**split_line(char *line);

// SECTION: lexer
char		**tokenize(char *source);

// SECTION: built-in commands
<<<<<<< HEAD
int			ft_echo(t_ast_node node);
int			ft_cd(t_ast_node node);
int			ft_pwd(void);
int			ft_exit(void);
void		ft_export(t_ast_node node, t_shell_data *shell_data);
int			ft_unset(void);
int			ft_env(void);
=======
int						ft_echo(t_ast_node node);
int						ft_cd(t_ast_node node, t_shell_data shell_data);
int						ft_pwd(void);
int						ft_exit(void);
int						ft_export(t_ast_node node, t_shell_data *shell_data);
int						ft_unset(t_ast_node node, t_shell_data *shell_data);
int						ft_env(t_shell_data shell_data);
>>>>>>> main

// SECTION: exec utils
int			is_builtin(char *cmd);
// SECTION: execution
char		**get_path_env_var(char **envp);
char		*validate_command(char *cmd, char **dirs);
int			execute_ast(t_ast_node *node, char **envp);
#endif
