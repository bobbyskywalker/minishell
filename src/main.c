/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/07 16:46:29 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	shell_loop(char **envp)
{
	char	*line;
	char	**tokens;

	line = NULL;
	while (1)
	{
		line = readline("$> ");
		tokens = tokenize(line);
		t_ast_node *node = build_ast(tokens);
		execute_ast(node, envp);
		// print_ast(node, 0);
		free_ast(node);
		free(line);
	}
}

// TODO: AST execution logic
// structure for minishell global data
// signal handling
// TESTS TESTS TESTS
int	main(int ac, char **av, char **envp)
{
	(void)envp;
	(void)av;
	if (ac != 1)
	{
		printf("valid exec.: ./minishell");
		return (0);
	}
	shell_loop(envp);
	return (EXIT_SUCCESS);
}
