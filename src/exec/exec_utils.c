/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:50:45 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 17:24:10 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/built_ins.h"
#include "../../inc/minishell.h"

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

int	exec_builtin(t_ast_node node, t_shell_data *shell_data)
{
	if (!ft_strncmp(node.command->args[0], "echo", 4))
		return (ft_echo(&node.command->args[1]));
	if (!ft_strncmp(node.command->args[0], "cd", 2))
		return (ft_cd(&node.command->args[1], *shell_data));
	if (!ft_strncmp(node.command->args[0], "pwd", 3))
		return (ft_pwd());
	if (!ft_strncmp(node.command->args[0], "exit", 4))
		return (ft_exit(shell_data), -1);
	if (!ft_strncmp(node.command->args[0], "export", 6))
		return (ft_export(&node.command->args[1], shell_data));
	if (!ft_strncmp(node.command->args[0], "unset", 5))
		return (ft_unset(&node.command->args[1], shell_data));
	if (!ft_strncmp(node.command->args[0], "env", 3))
		return (ft_env(*shell_data));
	return (1);
}

// return non-zero on error
int	prepare_cmd_for_exec(t_ast_node *node, t_shell_data *shell_data)
{
	char	**dirs;

	if (!node || node->type != COMMAND_NODE)
		return (-1);
	if (is_builtin(node->command->args[0]))
	{
		shell_data->last_cmd_status = exec_builtin(*node, shell_data);
		return (1);
	}
	dirs = get_path_env_var(shell_data->env_vars);
	node->command->args[0] = validate_command(node->command->args[0], dirs,
			shell_data);
	ft_arr2d_free(dirs);
	if (!node->command->args[0])
		return (-1);
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

int	swap_env_val(t_ast_node *node, t_shell_data shell_data)
{
	int	i;
	int	key_id;

	i = 0;
	while (node->command->args[i])
	{
		if (node->command->args[i][0] == '$')
		{
			if (node->command->args[i][1] == '?')
			{
				free(node->command->args[i]);
				node->command->args[i] = ft_itoa(shell_data.last_cmd_status);
				continue ;
			}
			key_id = is_key_in_envs(&node->command->args[i][1],
					shell_data.env_vars);
			free(node->command->args[i]);
			if (key_id == -1)
				node->command->args[i] = ft_strdup("");
			else
				node->command->args[i] = get_value(shell_data.env_vars[key_id]);
		}
		i++;
	}
	return (1);
}

void	process_env_vars(t_ast_node *node, t_shell_data shell_data)
{
	int	status;

	status = 0;
	if (!node)
		return ;
	if (node->type == COMMAND_NODE)
	{
		status = swap_env_val(node, shell_data);
		if (status < 0)
			return ;
	}
	process_env_vars(node->left_child, shell_data);
	process_env_vars(node->right_child, shell_data);
}
