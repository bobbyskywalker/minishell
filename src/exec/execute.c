/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/28 18:09:02 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// TODO: executable command search and path extraction
// signal handling


// returns negative value on errors:
// - tree non-existent
// - command execution failure
int execute_ast(t_ast_node *node)
{
    if (!node)
        return (-1);
    if (node->type == COMMAND_NODE)
        return execute_command(node);
    if (node->type == REDIRECT_NODE)
        return execute_redirection(node);
    if (node->type == PIPE_NODE)
        return execute_pipeline(node);
    return (-1);
}

int execute_command(t_ast_node *node)
{
    char **args = node->command->args;
	int status;
    pid_t pid;

    if (!node || node->type != COMMAND_NODE)
        return (-1);

    if (is_builtin(args[0])) 
        return execute_builtin(args);
    pid = fork();
    if (!pid)
    {
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
    return (-1);
}

int execute_redirection(t_ast_node *node)
{
	int	fd;

    if (!node || node->type != REDIRECT_NODE)
        return (-1);
    if (node->redirect->type == OUTPUT_REDIRECT)
        fd = open(node->redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (node->redirect->type == APPEND_REDIRECT)
        fd = open(node->redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (node->redirect->type == INPUT_REDIRECT)
        fd = open(node->redirect->filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return (-1);
    }
    if (node->redirect->type == INPUT_REDIRECT)
        dup2(fd, STDIN_FILENO);
    else
        dup2(fd, STDOUT_FILENO);
    close(fd);
    return execute_ast(node->left_child); // exec command after redirection
}

int execute_pipeline(t_ast_node *node)
{
	pid_t pid;
	int status;

    if (!node || node->type != PIPE_NODE)
        return (-1);
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    pid = fork();
    if (pid == 0) // left command dups (producer part of pipe)
    {
        close(pipe_fd[0]); // close read end
        dup2(pipe_fd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipe_fd[1]);
        exit(execute_ast(node->left_child)); // exec left command
    }

    pid_t pid2 = fork();
    if (pid2 == 0) // right command dups (consumer part of pipe)
    {
        close(pipe_fd[1]); // close write end
        dup2(pipe_fd[0], STDIN_FILENO); // redirect stdin to pipe
        close(pipe_fd[0]);

        exit(execute_ast(node->right_child)); // exec right command
    }
    // close the pipe
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    waitpid(pid2, &status, 0);
    return (WEXITSTATUS(status));
}
