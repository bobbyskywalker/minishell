/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:05:43 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/17 11:20:34 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/built_ins.h"

char	*get_key(char *str)
{
	size_t	i;
	size_t	j;
	char	*key;

	if (!str || !ft_strchr(str, '='))
		return (NULL);
	i = 0;
	while (str[i] != '=')
		i++;
	key = (char *)malloc(sizeof(char) * (i + 1));
	j = 0;
	while (j < i)
	{
		key[j] = str[j];
		j++;
	}
	key[j] = 0;
	return (key);
}

char	*get_value(char *str)
{
	char	*value;
	size_t	i;
	size_t	j;
	size_t	val_len;

	if (!str || !ft_strchr(str, '=') || !ft_strchr(str, '=')[1])
		return (NULL);
	i = 0;
	while (str[i] != '=')
		i++;
	val_len = ft_strlen(str) - (i + 1);
	value = (char *)malloc(sizeof(char) * (val_len + 1));
	j = 0;
	while (j < val_len)
	{
		value[j] = str[i + 1];
		j++;
		i++;
	}
	value[j] = 0;
	return (value);
}

int	compare_keys(char *str_a, char *str_b)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str_a[i] && str_a[i] != '=')
		i++;
	while (str_b[j] && str_b[j] != '=')
		j++;
	if (i != j)
		return (0);
	if (!ft_strncmp(str_a, str_b, i))
		return (1);
	return (0);
}

int	is_key_in_envs(char *str, char **envs)
{
	int		i;
	char	*key_str;
	char	*key_envs;

	key_str = get_key(str);
	if (!key_str)
		return (-1);
	i = 0;
	while (envs[i])
	{
		key_envs = get_key(envs[i]);
		if (!key_envs)
			return (free(key_str), -1);
		if (compare_keys(key_str, key_envs))
			return (free(key_envs), free(key_str), i);
		free(key_envs);
		i++;
	}
	free(key_str);
	return (-1);
}

int	swap_env_val(t_ast_node *node, t_shell_data s_d)
{
	int	i;
	int	key_id;

	i = -1;
	while (node->command->args[++i])
	{
		if (node->command->args[i][0] == '$')
		{
			if (node->command->args[i][1] == '?')
			{
				free(node->command->args[i]);
				node->command->args[i] = ft_itoa(s_d.last_cmd_status);
				continue ;
			}
			ft_strlcat(node->command->args[i], "=",
				ft_strlen(node->command->args[i]) + 2);
			key_id = is_key_in_envs(&node->command->args[i][1], s_d.env_vars);
			free(node->command->args[i]);
			if (key_id == -1)
				node->command->args[i] = ft_strdup("");
			else
				node->command->args[i] = get_value(s_d.env_vars[key_id]);
		}
	}
	return (1);
}
