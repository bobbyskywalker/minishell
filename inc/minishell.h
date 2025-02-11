#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/libft.h"
# include "parser.h"
# include "built_ins.h"

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

// SECTION: lexer
char		**tokenize(char *source);

// SECTION: redirections
void		restore_fds(int saved_stdin, int saved_stdout);
t_ast_node	*traverse_to_command(t_ast_node *node);
int			handle_out_app_redirections(t_ast_node *node, int fd,
				int saved_stdin);
int			handle_input_redirection(t_ast_node *node, int fd);
void		preprocess_heredocs(t_ast_node *node);
int			handle_heredoc(char *limiter);

// SECTION: pipes
int			kill_on_error(pid_t pid, int mode);
void		exec_first_child(int pipe_fd[2], t_ast_node *node, t_shell_data *shell_data);
void		exec_second_child(int pipe_fd[2], t_ast_node *node, t_shell_data *shell_data);
void		close_and_await(int pipe_fd[2], int pid1, int pid2, int status);

// SECTION: execution
char		**get_path_env_var(char **envp);
char		*validate_command(char *cmd, char **dirs);
int			prepare_cmd_for_exec(t_ast_node *node, char **envp);
int			calc_file_flags(t_ast_node *node);
int			execute_ast(t_ast_node *node, t_shell_data *shell_data);

// SECTION: exec utils
// int			is_builtin(t_ast_node node, t_shell_data *shell_data);
int			is_builtin(char *cmd);

#endif
