/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 14:49:35 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	shell_loop(void)
{
	char	*line;
	char	**tokens;

	line = NULL;
	while (1)
	{
		write(1, "$> ", 3);
		line = get_next_line(0);
		tokens = tokenize(line);
		t_ast_node *node = build_ast(tokens);
		print_ast(node, 0);
		free_ast(node);
		free(line);
	}
}

// TODO: Tokenizer norm fixes
// parser and interpreter to AST
// AST execution logic
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
	shell_loop();
	return (EXIT_SUCCESS);
}
