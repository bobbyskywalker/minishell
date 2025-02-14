/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:16:02 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/14 13:16:16 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cleanup(t_shell_data *shell_data)
{
	ft_arr2d_free(shell_data->env_vars);
	free_ast(shell_data->root);
	free(shell_data);
}
