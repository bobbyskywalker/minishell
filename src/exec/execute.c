/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/08 14:45:16 by agarbacz         ###   ########.fr       */
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
	char	**args;
	int		status;
	pid_t	pid;
	char	**dirs;

	args = node->command->args;
	if (!node || node->type != COMMAND_NODE)
		return (-1);
	if (is_builtin(args[0]))
		return (1);
	dirs = get_path_env_var(envp);
	node->command->args[0] = validate_command(node->command->args[0], dirs);
	if (!node->command->args[0])
		return (-1);
	// return (execute_builtin(args));
	pid = fork();
	ft_arr2d_free(dirs);
	if (!pid)
	{
		execve(node->command->args[0], args, envp);
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

int execute_redirection(t_ast_node *node, char **envp)
{
    int fd = -1;
    int saved_stdin = -1;
    int saved_stdout = -1;
    int status;
    t_ast_node *cmd_node = NULL;
    t_ast_node *current = node;

    if (!node || node->type != REDIRECT_NODE)
        return (-1);

    // traverse to find the last command node in redirections
    while (current && current->type == REDIRECT_NODE)
        current = current->left_child;
    cmd_node = current;  // this should be the command node
    if (!cmd_node)
        return (-1);
    // save original stdin and stdout
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    // handle input redirection
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
    else  // handle output redirection
    {
        int flags = O_WRONLY | O_CREAT;
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
    // exec cmd after redirections
    status = execute_ast(cmd_node, envp);
    // restore original stdin / stdout
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


int execute_pipeline(t_ast_node *node, char **envp)
{
    pid_t pid1, pid2;
    int status;
    int pipe_fd[2];

    if (!node || node->type != PIPE_NODE)
        return (-1);

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    // left side of pipe
    pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork");
        return (-1);
    }
    if (pid1 == 0)
    {
        close(pipe_fd[0]); // close read end
        dup2(pipe_fd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipe_fd[1]);
        exit(execute_ast(node->left_child, envp));
    }
    // second command (right side of pipe)
    pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork");
        kill(pid1, SIGTERM);
        return (-1);
    }
    if (pid2 == 0)
    {
        close(pipe_fd[1]);              // close write end
        dup2(pipe_fd[0], STDIN_FILENO); // redirect stdin from pipe
        close(pipe_fd[0]);
        // exec ast, dont handle redirs separately
        exit(execute_ast(node->right_child, envp));
    }
    // parent process
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
