/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/11 11:49:26 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// env var parsing
// if '$' before token
// swap with actual var value (NULL on non-existent)
void	shell_loop(t_shell_data *shell_data)
{
	char	*line;
	char	**tokens;

	// (void) shell_data;
	
	line = NULL;
	while (1)
	{
		line = readline("$> ");
		add_history(line);
		tokens = tokenize(line);
		t_ast_node *node = build_ast(tokens);
		execute_ast(node, shell_data);
		// print_ast(node, 0);
		free_ast(node);
		free(line);
	}
}

// TODO: Signal handling
// $?
// env variables
// search and lanuch executable
// here doc not printing and here doc redirecitons maybeeeee
// error handling?
// int	main(int ac, char **av, char **envp)
// {
// 	(void)envp;
// 	(void)av;
// 	if (ac != 1)
// 	{
// 		printf("valid exec.: ./minishell");
// 		return (0);
// 	}
// 	shell_loop(envp);
// 	return (EXIT_SUCCESS);
// }

// KUBA's MAIN
int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_shell_data *shell_data;
	
	shell_data = malloc(sizeof(t_shell_data));
	if (!shell_data)
		return (1);
	shell_data->env_vars = ft_2d_strdup(envp);
	
	// debug
	
	// for (int i = 0; shell_data->env_vars[i]; i++)
	// {
	// 	printf("%s\n", shell_data->env_vars[i]);
	// }
	if (ac != 1)
	{
		printf("valid exec.: ./minishell");
		return (1);
	}
	shell_loop(shell_data);
	ft_arr2d_free(shell_data->env_vars);
	free(shell_data);
	return (EXIT_SUCCESS);
}