/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 15:52:25 by agarbacz         ###   ########.fr       */
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

int	execute_redirection(t_ast_node *node, char **envp)
{
	int			fd;
	int			saved_stdin;
	int			saved_stdout;
	int			status;
	t_ast_node	*cmd_node;
	t_ast_node	*current;
	int			flags;

	fd = -1;
	saved_stdin = -1;
	saved_stdout = -1;
	cmd_node = NULL;
	current = node;
	if (!node || node->type != REDIRECT_NODE)
		return (-1);
	while (current && current->type == REDIRECT_NODE)
		current = current->left_child;
	cmd_node = current;
	if (!cmd_node)
		return (-1);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (node->redirect->type == INPUT_REDIRECT)
	{
		if (node->redirect->is_heredoc)
			fd = handle_heredoc(node->redirect->limiter);
		else
			fd = open(node->redirect->filename, O_RDONLY);
		if (fd == -1)
		{
			perror("open input");
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		flags = O_WRONLY | O_CREAT;
		flags |= (node->redirect->type == APPEND_REDIRECT) ? O_APPEND : O_TRUNC;
		fd = open(node->redirect->filename, flags, 0644);
		if (fd == -1)
		{
			perror("open output");
			if (saved_stdin != -1)
				dup2(saved_stdin, STDIN_FILENO);
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	status = execute_ast(cmd_node, envp);
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (status);
}

int	execute_pipeline(t_ast_node *node, char **envp)
{
	int	status;
	int	pipe_fd[2];

	pid_t pid1, pid2;
	if (!node || node->type != PIPE_NODE)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		exit(execute_ast(node->left_child, envp));
	}
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork");
		kill(pid1, SIGTERM);
		return (-1);
	}
	if (pid2 == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		exit(execute_ast(node->right_child, envp));
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_ast(t_ast_node *node, char **envp)
{
	if (!node)
		return (-1);
	preprocess_heredocs(node);
	if (node->type == REDIRECT_NODE)
		return (execute_redirection(node, envp));
	if (node->type == COMMAND_NODE)
		return (execute_command(node, envp));
	if (node->type == PIPE_NODE)
		return (execute_pipeline(node, envp));
	return (-1);
}
