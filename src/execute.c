/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:09:01 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/28 18:09:02 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exec_built_in(char *command)
{
    if (!ft_strncmp(command, "cd", 2))
        return (ft_cd());
    if (!ft_strncmp(command, "pwd", 3))
        return (ft_pwd());
    if (!ft_strncmp(command, "exit", 4))
        return (ft_exit());
    if (!ft_strncmp(command, "echo", 4))
        return (ft_echo());
    if (!ft_strncmp(command, "export", 6))
        return (ft_export());
    if (!ft_strncmp(command, "unset", 5))
        return (ft_unset());
    if (!ft_strncmp(command, "env", 3))
        return (ft_env());
    return (1);
}


void execute_command(char *command)
{
    if (exec_built_in(command))
        printf("command not found\n");
}
