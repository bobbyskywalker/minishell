/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/08 15:58:20 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	shell_loop(t_shell_data shell_data)
{
	char	*line;
	char	**tokens;

	line = NULL;
	while (1)
	{
		line = readline("$> ");
		tokens = tokenize(line);
		t_ast_node *node = build_ast(tokens);
		execute_ast(node, &shell_data);
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
	(void)av;
	t_shell_data shell_data;
	
	shell_data.env_vars = ft_2d_strdup(envp);
	if (ac != 1)
	{
		printf("valid exec.: ./minishell");
		return (0);
	}
	shell_loop(shell_data);
	ft_arr2d_free(shell_data.env_vars);
	return (EXIT_SUCCESS);
}
