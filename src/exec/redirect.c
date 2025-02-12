/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:10:36 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 17:11:33 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_input_redirection(t_ast_node *node, int fd)
{
	fd = open(node->redirect->filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open input");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirect(t_ast_node *current, t_ast_node *last_out,
		int saved_fds[2])
{
	int	tmp_fd;

	tmp_fd = open(current->redirect->filename, calc_file_flags(current), 0644);
	if (tmp_fd == -1)
	{
		restore_fds(saved_fds[0], saved_fds[1]);
		return (-1);
	}
	if (current == last_out)
	{
		if (dup2(tmp_fd, STDOUT_FILENO) == -1)
		{
			close(tmp_fd);
			restore_fds(saved_fds[0], saved_fds[1]);
			return (-1);
		}
	}
	close(tmp_fd);
	return (0);
}

void	get_last_in_and_out(t_ast_node *node, t_ast_node **last_in,
		t_ast_node **last_out)
{
	t_ast_node	*current;

	current = node;
	while (current && current->type == REDIRECT_NODE)
	{
		if (current->redirect->type == INPUT_REDIRECT)
			*last_in = current;
		else
			*last_out = current;
		current = current->left_child;
	}
}

int	apply_redirections(t_ast_node *node, t_ast_node *last_in,
		t_ast_node *last_out, int saved_fds[2])
{
	t_ast_node	*current;

	current = node;
	while (current && current->type == REDIRECT_NODE)
	{
		if (current->redirect->type != INPUT_REDIRECT)
		{
			if (handle_output_redirect(current, last_out, saved_fds) == -1)
				return (-1);
		}
		else if (current == last_in)
		{
			if (handle_input_redirection(current, -1) == -1)
			{
				restore_fds(saved_fds[0], saved_fds[1]);
				return (-1);
			}
		}
		current = current->left_child;
	}
	return (0);
}
