/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:09:36 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 13:32:52 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast_node	*parse_pipeline_part(char ***tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	char		*token;

	left = parse_cmd(tokens);
	left = parse_redirections_part(tokens, left);
	while (1)
	{
		token = get_next_token(tokens);
		if (!token)
			break ;
		if (!ft_strncmp(token, "|", 1))
		{
			right = parse_redirections_part(tokens, right);
			right = parse_cmd(tokens);
			left = create_operator_node(PIPE_NODE, left, right);
		}
		else
		{
			(*tokens)--;
			break ;
		}
	}
	return (left);
}

t_redirect_type	assign_redirection_type(char *token)
{
	t_redirect_type	type;

	if (!ft_strncmp(token, "<", 1) && ft_strncmp(token, "<<", 2))
		type = INPUT_REDIRECT;
	else if (!ft_strncmp(token, ">", 1) && ft_strncmp(token, ">>", 2))
		type = OUTPUT_REDIRECT;
	else if (!ft_strncmp(token, ">>", 2))
		type = APPEND_REDIRECT;
	else if (!ft_strncmp(token, "<<", 2))
		type = HEREDOC;
	else
		type = 0;
	return (type);
}

t_ast_node	*parse_redirections_part(char ***tokens, t_ast_node *node)
{
	t_redirect_type	type;
	char			*token;
	char			*filename;

	while (1)
	{
		token = get_next_token(tokens);
		if (!token)
			break ;
		if (!ft_strncmp(token, "<", 1) || !ft_strncmp(token, ">", 1)
			|| !ft_strncmp(token, ">>", 2) || !ft_strncmp(token, "<<", 2))
		{
			type = assign_redirection_type(token);
			filename = get_next_token(tokens);
			if (!filename)
				return (NULL);
			node = create_redirect_node(type, filename, node);
		}
		else
		{
			(*tokens)--;
			break ;
		}
	}
	return (node);
}
