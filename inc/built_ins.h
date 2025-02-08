/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:06:17 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/08 19:34:08 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "minishell.h"
# include "parser.h"


char	*get_key(char *str);
char	*get_value(char *str);
int		compare_keys(char *str_a, char *str_b);
int		is_key_in_envs(char *str, char **envs);

int		ft_echo(char **str);
int		ft_cd(char **args, t_shell_data shell_data);
int		ft_pwd();
int		ft_exit();
int		ft_env(t_shell_data shell_data);
int		ft_export(char **args, t_shell_data *shell_data);
int		ft_unset(char **args, t_shell_data *shell_data);

#endif