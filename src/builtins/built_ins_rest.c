/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_rest.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/06 16:18:43 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

int	ft_echo(t_ast_node node)
{
	size_t	i;
	int		is_flag;

	if (!node.command->args)
		return (-1);
	is_flag = 0;
	i = 0;
	if (!ft_strncmp(node.command->args[0], "-n", 3))
	{
		is_flag = 1;
		i++;
	}
	while (node.command->args[i])
	{
		ft_printf("%s", node.command->args[i]);
		i++;
	}
	if (!is_flag)
		ft_printf("\n");
	return (1);
}

int	ft_cd(t_ast_node node, t_shell_data shell_data)
{
	int		status;
	int		args_count;
	char	*path;
	size_t	id;

	args_count = ft_2d_strlen(node.command->args);
	if (args_count > 1)
	{
		ft_printf("cd: string not in pwd: %s", node.command->args[0]);
		return (0);
	}
	if (args_count < 1)
	{
		id = is_key_in_envs("HOME", shell_data.env_vars);
		if (id == -1)
			return (0);
		path = get_value(shell_data.env_vars[id]);
	}
	else
		path = node.command->args[0];
	status = chdir(path);
	if (status < 0)
		ft_printf("cd: no such file or directory: %s\n", path);
	return (1);
}

int	ft_pwd(void)
{
	char	*pwd_str;

	pwd_str = getcwd(NULL, 0);
	ft_printf("%s\n", pwd_str);
	free(pwd_str);
	return (0);
}

int	ft_exit(void)
{
	exit(1);
	return (0);
}

int	ft_env(t_shell_data shell_data)
{
	int	i;

	i = 0;
	if (!shell_data.env_vars)
		return (-1);
	while (shell_data.env_vars[i])
	{
		ft_printf("%s\n", shell_data.env_vars[i]);
		i++;
	}
	return (1);
}
