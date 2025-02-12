/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 10:13:11 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	shell_loop(t_shell_data *shell_data)
{
	char		*line;
	char		**tokens;
	t_ast_node	*node;

	
	line = NULL;
	while (1)
	{
		
		line = readline("minicfel $> ");
		if (!line)
			break;
		if (!*line)
			continue;
		printf("debuf\n");
		// if (!line)
		// 	continue;
		add_history(line);
		tokens = tokenize(line);
		if (!tokens || !*tokens || !**tokens)
			continue;
		node = build_ast(tokens);
		execute_ast(node, shell_data);
		// print_ast(node, 0);
		free_ast(node);
		free(line);
	}
	rl_clear_history();
}

// TODO:
// Signal handling
// here doc not printing and here doc redirecitons
// heredoc tmp_file not unlinking itself
// input redirections fixes
// output redirections fixes ?
// error handling?
// cleanup on exits
// memory leaks
// segfault on empty line
int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_shell_data *shell_data;

	handle_signals();
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
