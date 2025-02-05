/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/05 19:17:27 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/parser.h"

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

	ft_printf("path before: %s\n", getcwd(NULL, 0)); // debug
	args_count = ft_2d_strlen(node.command->args);
	if (args_count > 1)
		ft_printf("cd: string not in pwd: %s", path);
	if (args_count < 1)
		path = getenv("HOME");
	else
		path = node.command->args[0];
	status = chdir(path);
	if (status < 0)
		ft_printf("cd: no such file or directory: %s\n", path);
	ft_printf("path after: %s\n", getcwd(NULL, 0)); // debug
	return (0);
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

int	compare_keys(char *str_a, char *str_b)
{
	size_t	i;
	size_t	j;
	
	i = 0;
	j = 0;
	while(str_a[i] && str_a[i] != '=')
		i++;
	while(str_b[j] && str_b[j] != '=')
		j++;
	if (i != j)
		return (0);
	if (!ft_strncmp(str_a, str_b, i))
		return (1);
	return (0);
}

char	*get_key(char *str)
{
	int	i;
	int	j;
	char	*key;
	
	if (!str)
		return(NULL);
	i = 0;
	while (str[i] != 0 && str[i] != '=')
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
	int		i;
	int		j;
	
	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	printf("len: %d\n", ft_strlen(str) - (i + 1));
	value = (char *)malloc(sizeof(char) * (ft_strlen(str) - (i + 1)));
	j = 0;
	while(j < i)
	{
		value[j] = str[i];
		j++;
		i++;
	}
	value[j] = 0;
	return (value);
}

int	is_key_in_envs(char *str, char **envs)
{
	int		i;
	char	*key_str;
	char	*key_envs;
	int		value;

	key_str = get_key(str);
	if (!key_str)
		return (-1);	
	i = 0;
	while(envs[i])
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

char	**ft_realloc_2dstr(char **arr, size_t old_rows, size_t new_rows)
{
	char	**new_arr;
	int		i;

	if (!arr || !*arr)
		return (NULL);
	new_arr = (char **)ft_calloc(new_rows, sizeof(char *));
	if (!new_arr)
		return(NULL);
	i = 0;
	while (i < old_rows && i < new_rows)
	{
		new_arr[i] = arr[i];
		i++;
	}
	if (new_rows < old_rows)
		while(arr[i])
			free(arr[i++]);
	free(arr);
	return (new_arr);
}

// supposed to be in alphabetical order???
// in case of no args it works like ">env" - not like ">export"
int	ft_export(t_ast_node node, t_shell_data *shell_data)
{
	size_t	vars_count;
	size_t	i;
	int		id;
	char	*tmp;
	char	**tmp_2d;
	
	//tu sprawdzam czy w ogóle jest co exportować
	if (ft_2d_strlen(node.command->args) < 1)
		return (0);
	i = -1;
	//a tu czy nie ma jakiegoś pustego stringa
	while (node.command->args[++i])
	{
		if (ft_strlen(node.command->args[i]) == 0)
		{
			ft_printf("export: not valid in this context:\n");
			return (-1);
		}
	}
	i = -1;
	while (node.command->args[++i])
	{
		id = is_key_in_envs(node.command->args[i], shell_data->env_vars);
		//Tutaj czy ma znak '=' - jak nie to dodaję
		if (!ft_strchr(node.command->args[i], '='))
		{
			tmp = node.command->args[i];
			node.command->args[i] = ft_strjoin(tmp, "=");
			free(tmp);
		}
		//Tutaj czy jest już taki env zapisany - jak tak to nadpisuję (no chyba, że nowy nie ma wartości)
		else if (id >= 0)
		{
			free(shell_data->env_vars[id]);
			shell_data->env_vars[id] = node.command->args[i];
		}
		if (id == -1)
		{
			//A tu normalnie dodaję jak leci
			vars_count = ft_2d_strlen(shell_data->env_vars);
			shell_data->env_vars = ft_realloc_2dstr(shell_data->env_vars, vars_count + 1, vars_count + 2);
			shell_data->env_vars[vars_count] = node.command->args[i];
		}
	}
	return (1);
}

int	ft_unset(t_ast_node node, t_shell_data *shell_data)
{
	int	i;
	int	id;
	int	vars_count;
	
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
			ft_printf("unset: %s: invalid parameter name\n", node.command->args[i]);
			i++;
		}
		else
		{
			id = is_key_in_envs(node.command->args[i], shell_data->env_vars);
			vars_count = ft_2d_strlen(shell_data->env_vars);
			if (id >= 0)
			{
				while(shell_data->env_vars[id])
				{
					shell_data->env_vars[id] = shell_data->env_vars[id + 1];
					id++;
				}
				shell_data->env_vars[vars_count] = NULL;
				shell_data->env_vars = ft_realloc_2dstr(shell_data->env_vars, vars_count + 1, vars_count);
			}
		}
	}
	return (0);
}

int	ft_env(t_shell_data shell_data)
{
	int			i;

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

int main()
{
	char *x;
	
	printf("dupa");
	x = get_value("Test=Jakiśtest");
}

/* int	main(int argc, char **argv, char **envp)
{
	t_ast_node node;
	t_shell_data shell_data;
	
	// char *tmp[] = {"dupa ", "i ", "chuj", NULL};
	//char *tmp[] = {"CIPA=noico", "CzegoSieKurwa=Patrzysz", "Cebularzu=JEBANY", "Gunwo","CIPA=JAJCO", "Cebularzu", NULL};
	char *tmp[] = {"ZDOTDIR", "COLORTERM", "Test1", NULL};
	node.command = malloc(sizeof(t_command));
	if (!node.command)
		return (1);
	node.command->args = ft_2d_strdup(tmp);
	shell_data.env_vars = ft_2d_strdup(envp);
	// ft_echo(node);
	// ft_pwd();
	// ft_cd(node);
	// ft_env();
	//ft_export(node, &shell_data);
	//ft_2d_strprintf(shell_data.env_vars);
	//ft_unset(node, &shell_data);
	//ft_2d_strprintf(shell_data.env_vars);
	//ft_printf("val: %s\n", x);
	ft_arr2d_free(shell_data.env_vars);
	free(node.command->args);
	free(node.command);
	return (1);
} */
