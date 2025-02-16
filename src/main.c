/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kubaz <kubaz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/16 23:38:18 by kubaz            ###   ########.fr       */
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
		set_global_signals();
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
		free_ast(shell_data->root);
	}
}

t_shell_data	*create_shell_data(char **envp)
{
	t_shell_data	*shell_data;

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

// Known issues:
// ctrl-c gets exit status = 85, not 130
// export with no args works like env, not like export
// export with args works a bit different than OG one
// ctrl-c doesn't escape from heredoc
int	main(int ac, char **av, char **envp)
{
	t_shell_data	*shell_data;

	(void)av;
	if (ac != 1)
	{
		printf("valid exec.: ./minishell");
		return (1);
	}
	shell_data = create_shell_data(envp);
	shell_loop(shell_data);
	rl_clear_history();
	ft_arr2d_free(shell_data->env_vars);
	free(shell_data);
	return (EXIT_SUCCESS);
}
