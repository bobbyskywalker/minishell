/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:12:30 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/17 11:15:15 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_quotes(char *str)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (i < ft_strlen(str))
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
}

static void	remove_env(char *env, t_shell_data *shell_data)
{
	int	id;
	int	vars_count;

	id = is_key_in_envs(env, shell_data->env_vars);
	vars_count = ft_2d_strlen(shell_data->env_vars);
	if (id >= 0)
	{
		free(shell_data->env_vars[id]);
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
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (args[++i])
	{
		remove_quotes(args[i]);
		j = -1;
		while (args[i][++j])
		{
			if (!ft_isalnum(args[i][j]))
			{
				i++;
				break ;
			}
		}
		tmp = ft_strjoin(args[i], "=");
		remove_env(tmp, shell_data);
		free(tmp);
	}
	return (0);
}
