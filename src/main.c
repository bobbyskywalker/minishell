/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:03 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/13 12:20:56 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

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
		shell_data->tokens = tokens;
		free(line);
		if (!tokens || !*tokens || !**tokens)
			continue ;
		node = build_ast(tokens);
		shell_data->root = node;
		execute_ast(node, shell_data);
		// if (shell_data->last_cmd_status != 0)
		// 	free_tokens(tokens);
		// else
		free_ast(shell_data->root);
		free(tokens);
	}
}

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

// TODO:
// leaks with pipes
// echo fails
// multiple output redirections fixes
// error handling?
// memory leaks (tokens not freeable???)
// ctrl-z acts funny
// ctrl-c after cat acts funny
// ft_cd not freeing
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
	rl_clear_history();
	ft_arr2d_free(shell_data->env_vars);
	free(shell_data);
	return (EXIT_SUCCESS);
}
