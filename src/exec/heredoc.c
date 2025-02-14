/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 10:57:14 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/14 18:16:41 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*generate_tmp_filename(void)
{
	static int	count = 0;
	char		*num_str;
	char		*tmp_name;

	num_str = ft_itoa(count++);
	tmp_name = ft_strjoin("heredoc_tmp_", num_str);
	free(num_str);
	return (tmp_name);
}

static void	get_heredoc_input(t_redirect *redirect, int fd_tmp)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strncmp(line, redirect->limiter, ft_strlen(redirect->limiter)
				+ 1))
		{
			free(line);
			break ;
		}
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		free(line);
	}
}

static int	process_single_heredoc(t_redirect *redirect)
{
	int		fd_tmp;
	char	*tmp_filename;

	tmp_filename = generate_tmp_filename();
	fd_tmp = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_tmp == -1)
	{
		free(tmp_filename);
		return (-1);
	}
	get_heredoc_input(redirect, fd_tmp);
	close(fd_tmp);
	if (redirect->filename)
		free(redirect->filename);
	redirect->filename = tmp_filename;
	return (0);
}

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
		if (process_single_heredoc(node->redirect) == -1)
			return ;
	}
	preprocess_heredocs(node->left_child);
	preprocess_heredocs(node->right_child);
}

void	cleanup_heredoc_files(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == REDIRECT_NODE && node->redirect->is_heredoc)
		unlink(node->redirect->filename);
	cleanup_heredoc_files(node->left_child);
	cleanup_heredoc_files(node->right_child);
}
