/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:10:36 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 16:26:39 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	restore_fds(int saved_stdin, int saved_stdout)
{
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
}

t_ast_node	*traverse_to_command(t_ast_node *node)
{
	t_ast_node	*current;

	current = node;
	while (current && current->type == REDIRECT_NODE)
		current = current->left_child;
	return (current);
}

int	handle_out_app_redirections(t_ast_node *node, int fd, int saved_stdin)
{
	fd = open(node->redirect->filename, calc_file_flags(node), 0644);
	if (fd == -1)
	{
		perror("open output");
		if (saved_stdin != -1)
			dup2(saved_stdin, STDIN_FILENO);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_input_redirection(t_ast_node *node, int fd)
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
	return (0);
}
