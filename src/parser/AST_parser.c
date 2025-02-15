/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:27:53 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/14 13:20:58 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// highest level command parsing utility
// creates & returns a valid-type node
t_ast_node	*parse_cmd(char ***tokens)
{
	char	**args;
	int		i;
	char	*token;

	args = ft_calloc(MAX_ARGS, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (1)
	{
		token = get_next_token(tokens);
		if (!token)
			break ;
		if (!ft_strncmp(token, "|", 1) || !ft_strncmp(token, "<", 1)
			|| !ft_strncmp(token, ">", 1) || !ft_strncmp(token, ">>", 2)
			|| !ft_strncmp(token, "<<", 2))
		{
			(*tokens)--;
			break ;
		}
		args[i++] = token;
	}
	return (create_cmd_node(args));
}

// highest level parser function
// utilizes recursion to travel through
// the expression and build the AST
t_ast_node	*build_ast(char **tokens)
{
	t_ast_node	*node;

	node = parse_pipeline_part(&tokens);
	node = parse_redirections_part(&tokens, node);
	return (node);
}
