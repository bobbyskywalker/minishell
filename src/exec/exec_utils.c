/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:50:45 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/10 17:55:35 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/built_ins.h"
#include "../../inc/minishell.h"

// nie można wywołać ./minishell
// int	is_builtin(t_ast_node node, t_shell_data *shell_data)
// {
// 	if (!ft_strncmp(node.command->args[0], "echo", 4))
// 		return (ft_echo(&node.command->args[1]));
// 	if (!ft_strncmp(node.command->args[0], "cd", 2))
// 		return (ft_cd(&node.command->args[1], *shell_data));
// 	if (!ft_strncmp(node.command->args[0], "pwd", 3))
// 		return (ft_pwd());
// 	if (!ft_strncmp(node.command->args[0], "exit", 4))
// 		return (ft_exit());
// 	if (!ft_strncmp(node.command->args[0], "export", 6))
// 		return (ft_export(&node.command->args[1], shell_data));
// 	if (!ft_strncmp(node.command->args[0], "unset", 5))
// 		return (ft_unset(&node.command->args[1], shell_data));
// 	if (!ft_strncmp(node.command->args[0], "env", 3))
// 		return (ft_env(*shell_data));
// 	return (0);
// }

int	is_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", 4))
		return (1);
	if (!ft_strncmp(cmd, "cd", 2))
		return (1);
	if (!ft_strncmp(cmd, "pwd", 3))
		return (1);
	if (!ft_strncmp(cmd, "exit", 4))
		return (1);
	if (!ft_strncmp(cmd, "export", 6))
		return (1);
	if (!ft_strncmp(cmd, "unset", 5))
		return (1);
	if (!ft_strncmp(cmd, "env", 3))
		return (1);
	return (0);
}


// return non-zero on error
int	prepare_cmd_for_exec(t_ast_node *node, char **envp)
{
	char	**dirs;

	if (!node || node->type != COMMAND_NODE)
		return (-1);
	if (is_builtin(node->command->args[0]))
		return (1);
	// return (execute_builtin(args));
	dirs = get_path_env_var(envp);
	node->command->args[0] = validate_command(node->command->args[0], dirs);
	if (!node->command->args[0])
		return (-1);
	ft_arr2d_free(dirs);
	return (0);
}

int	calc_file_flags(t_ast_node *node)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (node->redirect->type == APPEND_REDIRECT)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}
