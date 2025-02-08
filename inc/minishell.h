/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 09:58:31 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/08 11:02:51 by agarbacz         ###   ########.fr       */
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

// SECTION: built-in commands

int			ft_echo(t_ast_node node);
int			ft_cd(t_ast_node node, t_shell_data shell_data);
int			ft_pwd(void);
int			ft_exit(void);
int			ft_export(t_ast_node node, t_shell_data *shell_data);
int			ft_unset(t_ast_node node, t_shell_data *shell_data);
int			ft_env(t_shell_data shell_data);

// SECTION: exec utils
int			is_builtin(char *cmd);
void		preprocess_heredocs(t_ast_node *node);
int			handle_heredoc(char *limiter);

// SECTION: execution
char		**get_path_env_var(char **envp);
char		*validate_command(char *cmd, char **dirs);
int			execute_ast(t_ast_node *node, char **envp);
#endif
