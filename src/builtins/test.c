/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:22:02 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/07 14:52:58 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/built_ins.h"

int	main(int argc, char **argv, char **envp)
{
	t_ast_node node;
	t_shell_data shell_data;
	
	// char *tmp[] = {"dupa ", "i ", "chuj", NULL};
	//char *tmp[] = {"CIPA=noico", "CzegoSieKurwa=Patrzysz", "Cebularzu=JEBANY", "Gunwo","CIPA=JAJCO", "Cebularzu", NULL};
	//char *tmp[] = {"ZDOTDIR", "COLORTERM", "Test1", NULL};
	char	*tmp[] = {".", NULL};
	node.command = malloc(sizeof(t_command));
	if (!node.command)
		return (1);
	node.command->args = ft_2d_strdup(tmp);
	shell_data.env_vars = ft_2d_strdup(envp);
	// ft_echo(node);
	ft_pwd();
	ft_cd(node, shell_data);
	ft_pwd();
	// ft_env();
	// ft_export(node, &shell_data);
	// ft_2d_strprintf(shell_data.env_vars);
	// ft_unset(node, &shell_data);
	// ft_2d_strprintf(shell_data.env_vars);
	//ft_printf("val: %s\n", x);
	ft_arr2d_free(shell_data.env_vars);
	ft_arr2d_free(node.command->args);
	free(node.command);
	return (1);
}