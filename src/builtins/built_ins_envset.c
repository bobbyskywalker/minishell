/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_envset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:11:17 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/14 12:15:08 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	remove_char(char *str, char c)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] != c)
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
}

static char	*process_args(char *input, t_shell_data shell_data)
{
	char	*processed_env;
	int		id;
	
	id = is_key_in_envs(input, shell_data.env_vars);
	// remove_char(input, '"');
	// ft_printf("removed: %s\n", input);
	if ((!ft_strchr(input, '=') || !&ft_strchr(input, '=')[1]) && id >= 0)
		return (NULL);
	if (!ft_strchr(input, '='))
		processed_env = ft_strjoin(input, "=");
	else
		processed_env = ft_strdup(input);
	return (processed_env);
}

static void	append_env(char *env, t_shell_data *shell_data)
{
	int		id;
	size_t	vars_count;

	if (!env)
		return ;
	id = is_key_in_envs(env, shell_data->env_vars);
	if (id >= 0)
	{
		free(shell_data->env_vars[id]);
		shell_data->env_vars[id] = env;
	}
	else
	{
		vars_count = ft_2d_strlen(shell_data->env_vars);
		shell_data->env_vars = ft_2dstr_realloc(shell_data->env_vars, vars_count
				+ 1, vars_count + 2);
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
		shell_data->env_vars = ft_2dstr_realloc(shell_data->env_vars, vars_count
				+ 1, vars_count);
	}
}

// supposed to be in alphabetical order???
// in case of no args it works like ">env" - not like ">export"
int	ft_export(char **args, t_shell_data *shell_data)
{
	size_t	i;
	char	*tmp;
	if (ft_2d_strlen(args) < 1)
		return (ft_env(*shell_data), 1);
	i = -1;
	while (args[++i])
		if (ft_strlen(args[i]) == 0)
			return (ft_printf("export: not valid in this context:\n"), -1);
	i = -1;
	while (args[++i])
	{
		tmp = process_args(args[i], *shell_data);
		// tmp = ft_strdup(args[i]);
		append_env(tmp, shell_data);
	}
	return (1);
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
