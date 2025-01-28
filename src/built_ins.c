/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:59 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/28 18:09:00 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int ft_echo(void)
{
  printf("here is echo command\n");
  return (0);
}

int ft_cd(void)
{
	printf("here is cd command\n");
  return (0);
}

int ft_pwd(void)
{
  printf("here is the pwd command\n");
  return (0);
}

int ft_exit(void)
{
  printf("here is exit command\n");
  return (0);
}

int ft_export(void)
{
  printf("here is export command\n");
  return (0);
}

int ft_unset(void)
{
  printf("here is the unset command\n");
  return (0);
}

int ft_env(void)
{
    printf("here is the env command\n");
    return (0);
}
