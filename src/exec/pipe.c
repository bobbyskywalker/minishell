/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:28:09 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/11 13:44:15 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	kill_on_error(pid_t pid, int mode)
{
	if (mode == 1)
	{
		if (pid < 0)
		{
			perror("fork");
			return (-1);
		}
	}
	else if (mode == 2)
	{
		if (pid < 0)
		{
			perror("fork");
			kill(pid, SIGTERM);
			return (-1);
		}
	}
	return (0);
}

void	exec_first_child(int pipe_fd[2], t_ast_node *node,
		t_shell_data *shell_data)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exit(execute_ast(node->left_child, shell_data));
}

void	exec_second_child(int pipe_fd[2], t_ast_node *node,
		t_shell_data *shell_data)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exit(execute_ast(node->right_child, shell_data));
}

void	close_and_await(int pipe_fd[2], int pid1, int pid2, int status)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
}
