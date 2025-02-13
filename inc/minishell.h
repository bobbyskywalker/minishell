/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:17:53 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/13 14:09:02 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# define MAX_ARGS 100

# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/libft.h"
# include "built_ins.h"
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
# include <termios.h>
# include <unistd.h>

// SECTION: env variables
int			swap_env_val(t_ast_node *node, t_shell_data shell_data);
void		process_env_vars(t_ast_node *node, t_shell_data shell_data);

// SECTION: redirections
void		restore_fds(int saved_stdin, int saved_stdout);
t_ast_node	*traverse_to_command(t_ast_node *node);
int			handle_dup_errors(int saved_stdin, int saved_stdout);
void		get_last_in_and_out(t_ast_node *node, t_ast_node **last_in,
				t_ast_node **last_out);
int			handle_output_redirect(t_ast_node *current, t_ast_node *last_out,
				int saved_fds[2]);
int			handle_input_redirection(t_ast_node *node, int fd);
int			apply_redirections(t_ast_node *node, t_ast_node *last_in,
				t_ast_node *last_out, int saved_fds[2]);
void		preprocess_heredocs(t_ast_node *node);
void		cleanup_heredoc_files(t_ast_node *node);

// SECTION: pipes
int			kill_on_error(pid_t pid, int mode);
void		exec_first_child(int pipe_fd[2], t_ast_node *node,
				t_shell_data *shell_data);
void		exec_second_child(int pipe_fd[2], t_ast_node *node,
				t_shell_data *shell_data);
void		close_and_await(int pipe_fd[2], int pid1, int pid2, int status);

// SECTION: execution
char		**get_path_env_var(char **envp);
char		*validate_command(char *cmd, char **dirs, t_shell_data *shell_data);
int			prepare_cmd_for_exec(t_ast_node *node, t_shell_data *shell_data);
int			calc_file_flags(t_ast_node *node);
int			execute_ast(t_ast_node *node, t_shell_data *shell_data);

// SECTION: exec utils
int			is_builtin(char *cmd);
int			exec_builtin(t_ast_node node, t_shell_data *shell_data);

// SECTION: signals handling
void		set_signals_handling(void);
void		reset_signals_handling(void);

#endif
