/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:27:53 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/13 12:07:54 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	// args[i] = NULL;
	return (create_cmd_node(args));
}

// highest level parser function
// utilizes recursion to travel
// through the expression and build the AST
t_ast_node	*build_ast(char **tokens)
{
	t_ast_node	*node;

	node = parse_pipeline_part(&tokens);
	node = parse_redirections_part(&tokens, node);
	return (node);
}

void	print_ast(t_ast_node *node, int depth)
{
	if (!node)
		return ;
	for (int i = 0; i < depth; i++)
		printf("  ");
	if (node->type == COMMAND_NODE)
	{
		printf("CMD: ");
		for (int i = 0; node->command->args[i]; i++)
			printf("%s ", node->command->args[i]);
		printf("\n");
	}
	else if (node->type == PIPE_NODE)
	{
		printf("PIPE\n");
	}
	else if (node->type == REDIRECT_NODE)
	{
		printf("REDIRECT (%s) -> %s\n",
			(node->redirect->type == INPUT_REDIRECT) ? "<" : (node->redirect->type == OUTPUT_REDIRECT) ? ">" : ">>",
			node->redirect->filename);
	}
	print_ast(node->left_child, depth + 1);
	print_ast(node->right_child, depth + 1);
}
