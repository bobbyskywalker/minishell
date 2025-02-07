/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:06:17 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/06 16:01:04 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "../../lib/libft/libft.h"
# include "../../inc/minishell.h"
# include "../../inc/parser.h"

char	*get_key(char *str);
char	*get_value(char *str);
int		compare_keys(char *str_a, char *str_b);
int		is_key_in_envs(char *str, char **envs);

int		ft_echo(t_ast_node node);
int		ft_cd(t_ast_node node, t_shell_data shell_data);
int		ft_pwd(void);
int		ft_exit(void);
int		ft_env(t_shell_data shell_data);
int		ft_export(t_ast_node node, t_shell_data *shell_data);
int		ft_unset(t_ast_node node, t_shell_data *shell_data);

#endif