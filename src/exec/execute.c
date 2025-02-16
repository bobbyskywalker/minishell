/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kubaz <kubaz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/16 23:31:45 by kubaz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// returns negative value on command execution failure
// prints redundant error message? idk
int	handle_cmd_errors(int cmd_status, t_shell_data *shell_data,
		t_ast_node *node)
{
	(void)node;
	if (cmd_status == -1)
	{
		shell_data->last_cmd_status = 127;
		//ft_printf("minishell: command not found: %s\n", node->command->args[0]);
		return (-1);
	}
	else if (cmd_status == 1)
		return (-1);
	return (0);
}

// highest level func responsible for command execution
// utilizes checks and validations for the executable
int	execute_command(t_ast_node *node, t_shell_data *shell_data)
{
	int		status;
	pid_t	pid;
	int		cmd_status;

	cmd_status = prepare_cmd_for_exec(node, shell_data);
	if (handle_cmd_errors(cmd_status, shell_data, node) != 0)
		return (-1);
	set_child_signals();
	pid = fork();
	if (!pid)
	{	
		execve(node->command->args[0], node->command->args,
			shell_data->env_vars);
		perror("execve");
		cleanup(shell_data);
		rl_clear_history();
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		shell_data->last_cmd_status = WEXITSTATUS(status);
		return (WEXITSTATUS(status));
	}
	return (-1);
}

// executes a redirection node
int	execute_redirection(t_ast_node *node, t_shell_data *shell_data, int status,
		t_ast_node *cmd_node)
{
	int			saved_fds[2];
	t_ast_node	*last_in;
	t_ast_node	*last_out;

	if (!node || node->type != REDIRECT_NODE)
		return (-1);
	cmd_node = traverse_to_command(node);
	if (cmd_node == NULL)
		return (-1);
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (handle_dup_errors(saved_fds[0], saved_fds[1]) == -1)
		return (-1);
	last_in = NULL;
	last_out = NULL;
	get_last_in_and_out(node, &last_in, &last_out);
	if (apply_redirections(node, last_in, last_out, saved_fds) == -1)
		return (-1);
	status = execute_ast(cmd_node, shell_data);
	restore_fds(saved_fds[0], saved_fds[1]);
	cleanup_heredoc_files(node);
	return (status);
}

// executes a pipe node using fork()
// to create child processes
// which execute the left and right leaves of the PIPE NODE
int	execute_pipeline(t_ast_node *node, t_shell_data *shell_data, int status)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (!node || node->type != PIPE_NODE)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid1 = fork();
	if (kill_on_error(pid1, 1) == -1)
		return (-1);
	if (pid1 == 0)
		exec_first_child(pipe_fd, node, shell_data);
	pid2 = fork();
	if (kill_on_error(pid2, 2) == -1)
		return (-1);
	if (pid2 == 0)
		exec_second_child(pipe_fd, node, shell_data);
	close_and_await(pipe_fd, pid1, pid2, status);
	return (WEXITSTATUS(status));
}

// highest level func responsible for parsed expression execution
// preprocesses heredocs & handles 
//environment variable substitution
int	execute_ast(t_ast_node *node, t_shell_data *shell_data)
{
	if (!node)
		return (-1);
	process_env_vars(node, *shell_data);
	preprocess_heredocs(node);
	if (node->type == REDIRECT_NODE)
		return (execute_redirection(node, shell_data, -1, NULL));
	if (node->type == COMMAND_NODE)
		return (execute_command(node, shell_data));
	if (node->type == PIPE_NODE)
		return (execute_pipeline(node, shell_data, -1));
	return (-1);
}
