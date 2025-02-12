/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_rest.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/12 18:02:36 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/built_ins.h"

int	ft_echo(char **args)
{
	size_t	i;
	int		is_flag;

	is_flag = 0;
	i = 0;
	if (!ft_strncmp(args[i], "-n", 3))
	{
		is_flag = 1;
		i++;
	}
	while (args[i])
	{
		while(*args[i])
		{
			if (*args[i] != '"')
				write(STDOUT_FILENO, args[i], 1);
			args[i]++;
		}
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!is_flag)
		ft_printf("\n");
	return (0);
}

int	ft_cd(char **args, t_shell_data shell_data)
{
	int		status;
	int		args_count;
	char	*path;
	int		id;

	args_count = ft_2d_strlen(args);
	if (args_count > 1)
		return (ft_printf("cd: string not in pwd: %s", args[0]), -1);
	if (args_count < 1)
	{
		id = is_key_in_envs("HOME", shell_data.env_vars);
		if (id == -1)
			return (1);
		path = get_value(shell_data.env_vars[id]);
	}
	else
		path = args[0];
	status = chdir(path);
	if (status < 0)
		return (ft_printf("cd: no such file or directory: %s\n", path), -1);
	return (1);
}

int	ft_pwd(void)
{
	char	*pwd_str;

	pwd_str = getcwd(NULL, 0);
	if (!pwd_str)
		return (0);
	ft_printf("%s\n", pwd_str);
	free(pwd_str);
	return (0);
}

void	ft_exit(t_shell_data *shell_data)
{
	ft_arr2d_free(shell_data->env_vars);
	free_ast(shell_data->root);
	free(shell_data);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

int	ft_env(t_shell_data shell_data)
{
	int	i;

	i = 0;
	if (!shell_data.env_vars)
		return (0);
	while (shell_data.env_vars[i])
	{
		ft_printf("%s\n", shell_data.env_vars[i]);
		i++;
	}
	return (0);
}
