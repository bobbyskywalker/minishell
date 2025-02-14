/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:12:30 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/14 16:13:21 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_env(char *env, t_shell_data *shell_data)
{
	int	id;
	int	vars_count;

	id = is_key_in_envs(env, shell_data->env_vars);
	vars_count = ft_2d_strlen(shell_data->env_vars);
	if (id >= 0)
	{
		while (shell_data->env_vars[id])
		{
			shell_data->env_vars[id] = shell_data->env_vars[id + 1];
			id++;
		}
		shell_data->env_vars[vars_count] = NULL;
		shell_data->env_vars = ft_2dstr_realloc(shell_data->env_vars, vars_count
				+ 1, vars_count);
	}
}

int	ft_unset(char **args, t_shell_data *shell_data)
{
	int	i;

	if (ft_2d_strlen(args) < 1)
	{
		ft_printf("unset: not enough arguments\n");
		return (1);
	}
	i = -1;
	while (args[++i])
	{
		if (ft_strchr(args[i], '='))
		{
			ft_printf("unset: %s: invalid parameter name\n", args[i]);
			i++;
		}
		else
			remove_env(args[i], shell_data);
	}
	return (1);
}
