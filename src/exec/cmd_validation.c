/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:29:58 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/14 12:33:26 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// function to extract the paths-to-executables
// from PATH env variable
char	**get_path_env_var(char **envp)
{
	char	**dirs;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			dirs = ft_split(envp[i] + 5, ':');
			return (dirs);
		}
		i++;
	}
	return (NULL);
}

void	set_error_status(char *cmd, t_shell_data *shell_data)
{
	if (ft_strchr(cmd, '/'))
		ft_printf("minishell: no such file or directory: %s\n", cmd);
	else
		ft_printf("minishell: command not found: %s\n", cmd);
	shell_data->last_cmd_status = 127;
}

// function to validate the input command presence & access
char	*validate_command(char *cmd, char **dirs, t_shell_data *shell_data)
{
	int			i;
	char		*cmd_path;
	char		*path;

	i = 0;
	while (dirs[i])
	{
		cmd_path = ft_strjoin(dirs[i], "/");
		path = ft_strjoin(cmd_path, cmd);
		free(cmd_path);
		if (!access(path, F_OK))
		{
			shell_data->last_cmd_status = 0;
			free(cmd);
			return (path);
		}
		if (!access(cmd, F_OK))
		{
			shell_data->last_cmd_status = 0;
			free(path);
			return (cmd);
		}
		free(path);
		i++;
	}
	set_error_status(cmd, shell_data);
	return (cmd);
}
