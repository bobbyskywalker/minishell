/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:04:09 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 17:11:09 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// restores the stdin and stdout file descriptors
// to its default values
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

// traverses the AST to find the command node
t_ast_node	*traverse_to_command(t_ast_node *node)
{
	t_ast_node	*current;

	current = node;
	while (current && current->type == REDIRECT_NODE)
		current = current->left_child;
	return (current);
}

// checks if the dup2 call was successful
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
