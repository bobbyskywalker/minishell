/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/11 15:48:59 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	shell_loop(t_shell_data *shell_data)
{
	char		*line;
	char		**tokens;
	t_ast_node	*node;

	// (void) shell_data;
	line = NULL;
	while (1)
	{
		line = readline("$> ");
		add_history(line);
		tokens = tokenize(line);
		node = build_ast(tokens);
		execute_ast(node, shell_data);
		// print_ast(node, 0);
		free_ast(node);
		free(line);
	}
	rl_clear_history();
}

// TODO: Signal handling
// search and lanuch executable
// here doc not printing and here doc redirecitons maybeeeee
// error handling?

int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_shell_data *shell_data;

	shell_data = malloc(sizeof(t_shell_data));
	if (!shell_data)
		return (1);
	shell_data->env_vars = ft_2d_strdup(envp);
	shell_data->last_cmd_status = 0;
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