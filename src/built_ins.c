/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/30 15:41:26 by jzackiew         ###   ########.fr       */
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

int	ft_cd(void)
{
	printf("here is cd command\n");
	return (0);
}

int	ft_pwd(void)
{
	char	*pwd_str;
	
	pwd_str = getcwd(NULL, 99);
	ft_printf("%s\n", pwd_str);
	return (0);
}

int	ft_exit(void)
{
	printf("here is exit command\n");
	return (0);
}

int	ft_export(void)
{
	printf("here is export command\n");
	return (0);
}

int	ft_unset(void)
{
	printf("here is the unset command\n");
	return (0);
}

int	ft_env(void)
{
	printf("here is the env command\n");
	return (0);
}

int	main(void)
{
	t_ast_node node;
	char *tmp[] = {"dupa ", "i ", "chuj", NULL};

	node.command = malloc(sizeof(t_command));
	if (!node.command)
		return (1);
	node.command->args = ft_2d_strdup(tmp);
	ft_echo(node);
	ft_pwd();
	return (1);
}