/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:56:09 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/08 19:45:17 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/built_ins.h"
#include "../../inc/minishell.h"


// echo nie stawia spacji
// nie można wywołać ./minishell
int	is_builtin(t_ast_node node, t_shell_data *shell_data)
{
	if (!ft_strncmp(node.command->args[0], "echo", 4))
		return (ft_echo(&node.command->args[1]));
	if (!ft_strncmp(node.command->args[0], "cd", 2))
		return (ft_cd(&node.command->args[1], *shell_data));
	if (!ft_strncmp(node.command->args[0], "pwd", 3))
		return (ft_pwd());
	if (!ft_strncmp(node.command->args[0], "exit", 4))
		return (ft_exit());
	if (!ft_strncmp(node.command->args[0], "export", 6))
		return (ft_export(&node.command->args[1], shell_data));
	if (!ft_strncmp(node.command->args[0], "unset", 5))
		return (ft_unset(&node.command->args[1], shell_data));
	if (!ft_strncmp(node.command->args[0], "env", 3))
		return (ft_env(*shell_data));
	return (0);
}
