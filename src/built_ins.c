/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 18:40:23 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_echo(t_ast_node node)
{
	size_t	i;
	int		is_flag;

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
	return (0);
}

int	ft_cd(t_ast_node node)
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

char	*get_key(char *env)
{
	size_t	i;
	char	*key;
	
	i = 0;
	while(env[i])
	{
		
		i++;
	}
}

int	compare_keys(char *str_a, char *str_b)
{
	size_t	i;
	size_t	j;
	
	i = 0;
	j = 0;
	while(str_a[i] || str_a[i] != '=')
		i++;
	while(str_b[j] || str_b[j] != '=')
		j++;
	if (i != j)
		return (0);
	if (ft_strncmp(str_a, str_b, i))
		return (0);
	return (1);
}

int	is_key_in_envs(char *str, char **envs)
{
	size_t	i;
	char	*key;
	
	key = ft_split(str, '=')[0];
	i = -1;
	while(envs[++i])
	{
		if (compare_keys(key, envs[i]))
			return (i);
	}
	free(key);
	return (-1);
}

// supposed to be in alphabetical order???
// in case of no args it works like ">env" - not like ">export"
void	ft_export(t_ast_node node, t_shell_data *shell_data)
{
	size_t	vars_count;
	size_t	i;

	if (ft_2d_strlen(shell_data->env_vars) < 1)
	{
		ft_2d_strprintf(shell_data->env_vars);
		return ;
	}
	i = -1;
	while (node.command->args[++i])
	{
		if (!ft_strchr(node.command->args[i], '='))
			node.command->args[i] = ft_strjoin(node.command->args[i], "=''");
		if (is_key_in_envs(node.command->args[i], shell_data->env_vars))
			

		vars_count = ft_2d_strlen(shell_data->env_vars);
		ft_2d_realloc(shell_data->env_vars, vars_count * sizeof(char *), (vars_count + 1) * sizeof(char *));
		shell_data->env_vars[vars_count] = node.command->args[i];
		shell_data->env_vars[vars_count + 1] = NULL;
	}
}

int	ft_unset(void)
{
	printf("here is the unset command\n");
	return (0);
}

int	ft_env(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		ft_printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_ast_node node;
	t_shell_data shell_data;
	// char *tmp[] = {"dupa ", "i ", "chuj", NULL};
	char *tmp[] = {"CIPA=noico", "CzegoSieKurwa=Patrzysz", "Cebularzu=JEBANY", "Gunwo", NULL};
	node.command = malloc(sizeof(t_command));
	if (!node.command)
		return (1);
	node.command->args = ft_2d_strdup(tmp);
	shell_data.env_vars = envp;
	// ft_echo(node);
	// ft_pwd();
	// ft_cd(node);
	// ft_env();
	ft_export(node, &shell_data);
	ft_2d_strprintf(shell_data.env_vars);
	//ft_printf("%s\n", getenv("CIPA"));
	//free(node.command->args);
	//free(node.command);
	return (1);
}
