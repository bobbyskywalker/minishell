/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/08 19:45:40 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// TODO: here_doc fixes
// Built-in execution
// signal handling

// returns negative value on errors:
// - tree non-existent
// - command execution failure
int	execute_command(t_ast_node *node, t_shell_data *shell_data)
{
	char	**args;
	int		status;
	pid_t	pid;
	char	**dirs;

	if (!node || node->type != COMMAND_NODE)
		return (-1);
	if (is_builtin(*node, shell_data))
		return (1);
	dirs = get_path_env_var(shell_data->env_vars);
	node->command->args[0] = validate_command(node->command->args[0], dirs);
	if (!node->command->args[0])
		return (-1);
	// return (execute_builtin(args));
	pid = fork();
	ft_arr2d_free(dirs);
	if (!pid)
	{
		execve(node->command->args[0], args, shell_data->env_vars);
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

int	handle_heredoc(char *limiter)
{
	char	*line;
	int		fd_tmp;

	fd_tmp = open("tmp_file", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd_tmp == -1)
	{
		perror("open");
		return (-1);
	}
	rl_clear_history();
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (!ft_strncmp(line, limiter, strlen(limiter)))
		{
			free(line);
			break;
		}
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		free(line);
	}
	close(fd_tmp);
	fd_tmp = open("tmp_file", O_RDONLY, 0644);
	if (fd_tmp == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd_tmp);
}

int	execute_redirection(t_ast_node *node, t_shell_data *shell_data)
{
	int	fd;
	int	saved_fd;
	int	status;

	fd = -1;
	if (!node || node->type != REDIRECT_NODE)
		return (-1);
	if (node->redirect->type == OUTPUT_REDIRECT)
		fd = open(node->redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redirect->type == APPEND_REDIRECT)
		fd = open(node->redirect->filename, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else if (node->redirect->type == INPUT_REDIRECT)
		fd = open(node->redirect->filename, O_RDONLY);
	else if (node->redirect->type == HEREDOC)
		fd = handle_heredoc(node->redirect->filename);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	if (node->redirect->type == INPUT_REDIRECT
		|| node->redirect->type == HEREDOC)
		saved_fd = dup(STDIN_FILENO);
	else
		saved_fd = dup(STDOUT_FILENO);
	if (node->redirect->type == INPUT_REDIRECT
		|| node->redirect->type == HEREDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	status = execute_ast(node->left_child, shell_data);
	if (node->redirect->type == INPUT_REDIRECT
		|| node->redirect->type == HEREDOC)
		dup2(saved_fd, STDIN_FILENO);
	else
		dup2(saved_fd, STDOUT_FILENO);
	close(saved_fd);
	if (node->redirect->type == HEREDOC)
		unlink("tmp_file");
	return (status);
}

int	execute_pipeline(t_ast_node *node, t_shell_data *shell_data)
{
	pid_t	pid;
	int		status;
	int		pipe_fd[2];
	pid_t	pid2;

	if (!node || node->type != PIPE_NODE)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0) // left command dups (producer part of pipe)
	{
		close(pipe_fd[0]);               // close read end
		dup2(pipe_fd[1], STDOUT_FILENO); // redirect stdout to pipe
		close(pipe_fd[1]);
		exit(execute_ast(node->left_child, shell_data)); // exec left command
	}
	pid2 = fork();
	if (pid2 == 0) // right command dups (consumer part of pipe)
	{
		close(pipe_fd[1]);              // close write end
		dup2(pipe_fd[0], STDIN_FILENO); // redirect stdin to pipe
		close(pipe_fd[0]);
		exit(execute_ast(node->right_child, shell_data)); // exec right command
	}
	// close the pipe
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_ast(t_ast_node *node, t_shell_data *shell_data)
{
	if (!node)
		return (-1);
	if (node->type == COMMAND_NODE)
		return (execute_command(node, shell_data));
	if (node->type == REDIRECT_NODE)
		return (execute_redirection(node, shell_data));
	if (node->type == PIPE_NODE)
		return (execute_pipeline(node, shell_data));
	return (-1);
}
