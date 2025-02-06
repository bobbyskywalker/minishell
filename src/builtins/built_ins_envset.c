/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_envset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:11:17 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/06 16:19:16 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

static void	append_env(char *env, t_shell_data *shell_data)
{
	int		id;
	char	*tmp;
	size_t	vars_count;

	id = is_key_in_envs(env, shell_data->env_vars);
	if (!ft_strchr(env, '=') && id == -1)
	{
		tmp = env;
		env = ft_strjoin(tmp, "=");
		free(tmp);
	}
	else if (id >= 0)
	{
		free(shell_data->env_vars[id]);
		shell_data->env_vars[id] = env;
	}
	if (id == -1)
	{
		vars_count = ft_2d_strlen(shell_data->env_vars);
		shell_data->env_vars = ft_2dstr_realloc(shell_data->env_vars,
				vars_count + 1, vars_count + 2);
		shell_data->env_vars[vars_count] = env;
	}
}

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
		shell_data->env_vars = ft_2dstr_realloc(shell_data->env_vars,
				vars_count + 1, vars_count);
	}
}

// supposed to be in alphabetical order???
// in case of no args it works like ">env" - not like ">export"
int	ft_export(t_ast_node node, t_shell_data *shell_data)
{
	size_t	i;
	char	*tmp;
	char	**tmp_2d;

	if (ft_2d_strlen(node.command->args) < 1)
		return (ft_env(*shell_data), 1);
	i = -1;
	while (node.command->args[++i])
		if (ft_strlen(node.command->args[i]) == 0)
			return (ft_printf("export: not valid in this context:\n"), -1);
	i = -1;
	while (node.command->args[++i])
		append_env(node.command->args[i], shell_data);
	return (1);
}

int	ft_unset(t_ast_node node, t_shell_data *shell_data)
{
	int	i;

	if (ft_2d_strlen(node.command->args) < 1)
	{
		ft_printf("unset: not enough arguments\n");
		return (0);
	}
	i = -1;
	while (node.command->args[++i])
	{
		if (ft_strchr(node.command->args[i], '='))
		{
			ft_printf("unset: %s: invalid parameter name\n",
				node.command->args[i]);
			i++;
		}
		else
			remove_env(node.command->args[i], shell_data);
	}
	return (0);
}
