/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 16:59:25 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// TODO: here_doc redirection
// file to file redirection
// Built-in execution
// signal handling

// returns negative value on errors:
// - tree non-existent
// - command execution failure
int	execute_command(t_ast_node *node, char **envp)
{
	int		status;
	pid_t	pid;

	if (prepare_cmd_for_exec(node, envp))
		return (-1);
	pid = fork();
	if (!pid)
	{
		execve(node->command->args[0], node->command->args, envp);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (-1);
}

int	execute_redirection(t_ast_node *node, char **envp, int status,
		t_ast_node *cmd_node)
{
	int	fd;
	int	saved_stdin;
	int	saved_stdout;

	fd = -1;
	if (!node || node->type != REDIRECT_NODE)
		return (-1);
	cmd_node = traverse_to_command(node);
	if (cmd_node == NULL)
		return (-1);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (node->redirect->type == INPUT_REDIRECT)
	{
		if (handle_input_redirection(node, fd) == -1)
			return (-1);
	}
	else
	{
		if (handle_out_app_redirections(node, fd, saved_stdin) == -1)
			return (-1);
	}
	status = execute_ast(cmd_node, envp);
	restore_fds(saved_stdin, saved_stdout);
	return (status);
}

int	execute_pipeline(t_ast_node *node, char **envp, int status)
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
		exec_first_child(pipe_fd, node, envp);
	pid2 = fork();
	if (kill_on_error(pid2, 2) == -1)
		return (-1);
	if (pid2 == 0)
		exec_second_child(pipe_fd, node, envp);
	close_and_await(pipe_fd, pid1, pid2, status);
	return (WEXITSTATUS(status));
}

int	execute_ast(t_ast_node *node, char **envp)
{
	if (!node)
		return (-1);
	preprocess_heredocs(node);
	if (node->type == REDIRECT_NODE)
		return (execute_redirection(node, envp, -1, NULL));
	if (node->type == COMMAND_NODE)
		return (execute_command(node, envp));
	if (node->type == PIPE_NODE)
		return (execute_pipeline(node, envp, -1));
	return (-1);
}
