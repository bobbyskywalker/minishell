/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 09:58:31 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 16:51:31 by agarbacz         ###   ########.fr       */
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

// SECTION: redirections
void		restore_fds(int saved_stdin, int saved_stdout);
t_ast_node	*traverse_to_command(t_ast_node *node);
int			handle_out_app_redirections(t_ast_node *node, int fd,
				int saved_stdin);
int			handle_input_redirection(t_ast_node *node, int fd);

// SECTION: pipes
int			kill_on_error(pid_t pid, int mode);
void		exec_first_child(int pipe_fd[2], t_ast_node *node, char **envp);
void		exec_second_child(int pipe_fd[2], t_ast_node *node, char **envp);
void		close_and_await(int pipe_fd[2], int pid1, int pid2, int status);

// SECTION: execution
char		**get_path_env_var(char **envp);
char		*validate_command(char *cmd, char **dirs);
int			prepare_cmd_for_exec(t_ast_node *node, char **envp);
int			calc_file_flags(t_ast_node *node);
int			execute_ast(t_ast_node *node, char **envp);
#endif
