/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 10:25:30 by jzackiew         ###   ########.fr       */
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

	ft_printf("path before: %s\n", getcwd(NULL, 0)); //debug
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
	ft_printf("path after: %s\n", getcwd(NULL, 0)); //debug
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

// supposed to be in alphabetical order???
// in case of no args it works like ">env" - not like ">export"
int	ft_export(t_ast_node node)
{
	extern char	**environ;
	char		**argv;
	int			args_count;

	args_count = ft_2d_strlen(node.command->args);
	if (args_count < 1)
		ft_2d_strprintf(environ);
	else
	{
		argv = (char **)ft_calloc(sizeof(char *), 4);
		if (!argv)
			exit(-1);
		argv[0] = "/bin/sh";
		argv[1] = "-c";
		argv[2] = ft_strjoin("export ",node.command->args[0]);
		execve("/bin/sh", argv, environ);
		perror("execve");
		ft_arr2d_free(argv);
	}
	return (0);
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

int	main(void)
{
	t_ast_node node;
	// char *tmp[] = {"dupa ", "i ", "chuj", NULL};
	char *tmp[] = {"CIPA=noico", NULL};
	node.command = malloc(sizeof(t_command));
	if (!node.command)
		return (1);
	node.command->args = ft_2d_strdup(tmp);
	// ft_echo(node);
	// ft_pwd();
	//ft_cd(node);
	//ft_env();
	ft_export(node);
	ft_printf("%s\n", getenv("CIPA"));
	free(node.command->args);
	free(node.command);
	return (1);
}