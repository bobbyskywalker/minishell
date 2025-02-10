/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 10:57:14 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 14:28:00 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// the function to identify heredocs
// and transform them into input redirections
// in order to prevent fd conflicts
void	preprocess_heredocs(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == REDIRECT_NODE && node->redirect->type == HEREDOC)
	{
		node->redirect->type = INPUT_REDIRECT;
		node->redirect->is_heredoc = true;
		if (node->redirect->limiter_assigned == false)
		{
			node->redirect->limiter_assigned = true;
			node->redirect->limiter = ft_strdup(node->redirect->filename);
		}
		free(node->redirect->filename);
		node->redirect->filename = ft_strdup("tmp_file");
	}
	preprocess_heredocs(node->left_child);
	preprocess_heredocs(node->right_child);
}

void	read_heredoc_input(char *line, char *limiter, int fd_tmp)
{
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strncmp(line, limiter, ft_strlen(limiter)))
		{
			free(line);
			break ;
		}
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(char *limiter)
{
	char	*line;
	int		fd_tmp;

	line = NULL;
	fd_tmp = open("tmp_file", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd_tmp == -1)
	{
		perror("open");
		return (-1);
	}
	read_heredoc_input(line, limiter, fd_tmp);
	close(fd_tmp);
	fd_tmp = open("tmp_file", O_RDONLY, 0644);
	if (fd_tmp == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd_tmp);
}
