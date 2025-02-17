/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:11:17 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/17 10:26:41 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_char(char *str, char c)
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

static int	check_input(char **input)
{
	int		i;
	int		j;
	char	*key;

	i = -1;
	while (input[++i])
	{
		if (input[i][0] == '=')
			return (ft_printf("export: not valid in this context:\n"), 1);
		key = get_key(input[i]);
		if (!key)
			continue ;
		j = -1;
		while (key[++j])
		{
			if (!ft_isalnum(key[j]) && key[j] != '=')
			{
				free(key);
				return (ft_printf("export: `%s': not a valid identifier\n",
						input[i]), 1);
			}
		}
		free(key);
		remove_char(input[i], '"');
		remove_char(input[i], '\'');
	}
	return (0);
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

static char	*merge_tokens(char *cur, char *next)
{
	char	*result;
	char	*tmp;

	if (!cur)
		return (NULL);
	if (!next)
		return (cur);
	tmp = ft_strjoin(cur, " ");
	result = ft_strjoin(tmp, next);
	free(tmp);
	free(cur);
	return (result);
}

int	ft_export(char **args, t_shell_data *shell_data)
{
	int		i;
	char	*last_key;

	if (ft_2d_strlen(args) < 1)
		return (ft_env(*shell_data), 0);
	if (check_input(args))
		return (1);
	last_key = NULL;
	i = -1;
	while (args[++i])
	{
		if (ft_strchr(args[i], '='))
			last_key = ft_strdup(args[i]);
		while (args[i + 1] && !ft_strchr(args[i + 1], '=') && last_key)
		{
			last_key = merge_tokens(last_key, args[i + 1]);
			i++;
		}
		append_env(last_key, shell_data);
	}
	return (0);
}
