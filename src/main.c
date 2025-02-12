/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 13:38:27 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	shell_loop(t_shell_data *shell_data)
{
	char		*line;
	char		**tokens;
	t_ast_node	*node;

	node = NULL;
	while (1)
	{
		line = readline("minicfel $> ");
		if (!line)
			break ;
		if (!*line)
			continue ;
		add_history(line);
		tokens = tokenize(line);
		free(line);
		if (!tokens || !*tokens || !**tokens)
			continue ;
		node = build_ast(tokens);
		shell_data->root = node;
		free(tokens);
		execute_ast(node, shell_data);
		if (node != shell_data->root)
			free_ast(node);
	}
	rl_clear_history();
}

// TODO:
// here doc not printing and here doc redirecitons
// heredoc tmp_file not unlinking itself
// input redirections fixes
// output redirections fixes ?
// error handling?
// cleanup on exits
// memory leaks
// ctrl-z lol

t_shell_data	*create_shell_data(char **envp)
{
	t_shell_data *shell_data;

	shell_data = malloc(sizeof(t_shell_data));
	if (!shell_data)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	shell_data->env_vars = ft_2d_strdup(envp);
	shell_data->last_cmd_status = 0;
	shell_data->root = NULL;
	return (shell_data);
}

int	main(int ac, char **av, char **envp)
{
	t_shell_data	*shell_data;

	(void)av;
	if (ac != 1)
	{
		printf("valid exec.: ./minishell");
		return (1);
	}
	handle_signals();
	shell_data = create_shell_data(envp);
	shell_loop(shell_data);
	free_ast(shell_data->root);
	ft_arr2d_free(shell_data->env_vars);
	free(shell_data);
	return (EXIT_SUCCESS);
}
