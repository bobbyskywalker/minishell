/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 16:32:48 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// returns negative value on errors:
// - tree non-existent
// - command execution failure
int	execute_command(t_ast_node *node, t_shell_data *shell_data)
{
	int		status;
	pid_t	pid;

	if (prepare_cmd_for_exec(node, shell_data))
		return (-1);
	pid = fork();
	if (!pid)
	{
		execve(node->command->args[0], node->command->args,
			shell_data->env_vars);
		perror("execve");
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

int	apply_single_redirection(t_ast_node *node)
{
	int	fd;

	fd = -1;
	if (node->redirect->type == INPUT_REDIRECT)
	{
		if (handle_input_redirection(node, fd) == -1)
			return (-1);
	}
	else
	{
		if (handle_out_app_redirections(node, fd, -1) == -1)
			return (-1);
	}
	return (0);
}
int	handle_dup_errors(int saved_stdin, int saved_stdout)
{
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		if (saved_stdin != -1)
			close(saved_stdin);
		if (saved_stdout != -1)
			close(saved_stdout);
		return (-1);
	}
	return (0);
}

int	get_last_in_out(t_ast_node *current, t_ast_node **last_in, t_ast_node **last_out)
{
	while (current && current->type == REDIRECT_NODE)
	{
		if (current->redirect->type == INPUT_REDIRECT)
		{
			if (!last_in)
				last_in = current;
		}
		else
		{
			if (!last_out)
				last_out = current;
		}
		current = current->left_child;
	}
}

int	execute_redirection(t_ast_node *node, t_shell_data *shell_data, int status,
		t_ast_node *cmd_node)
{
	int			saved_stdin;
	int			saved_stdout;
	t_ast_node	*current;
	t_ast_node	*last_in;
	t_ast_node	*last_out;
	int			flags;
	int			tmp_fd;

	if (!node || node->type != REDIRECT_NODE)
		return (-1);
	cmd_node = traverse_to_command(node);
	if (cmd_node == NULL)
		return (-1);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_dup_errors(saved_stdin, saved_stdout) == -1)
		return (-1);
	last_in = NULL;
	last_out = NULL;
	current = node;
	while (current && current->type == REDIRECT_NODE)
	{
		if (current->redirect->type == INPUT_REDIRECT)
		{
			if (!last_in)
				last_in = current;
		}
		else
		{
			if (!last_out)
				last_out = current;
		}
		current = current->left_child;
	}
	current = node;
	while (current && current->type == REDIRECT_NODE)
	{
		if (current->redirect->type != INPUT_REDIRECT)
		{
			flags = calc_file_flags(current);
			tmp_fd = open(current->redirect->filename, flags, 0644);
			if (tmp_fd == -1)
			{
				restore_fds(saved_stdin, saved_stdout);
				return (-1);
			}
			if (current == last_out)
			{
				if (dup2(tmp_fd, STDOUT_FILENO) == -1)
				{
					close(tmp_fd);
					restore_fds(saved_stdin, saved_stdout);
					return (-1);
				}
			}
			close(tmp_fd);
		}
		else if (current == last_in)
		{
			if (handle_input_redirection(current, -1) == -1)
			{
				restore_fds(saved_stdin, saved_stdout);
				return (-1);
			}
		}
		current = current->left_child;
	}
	status = execute_ast(cmd_node, shell_data);
	restore_fds(saved_stdin, saved_stdout);
	current = node;
	cleanup_heredoc_files(current);
	return (status);
}

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
