/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:51 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/28 19:41:40 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <assert.h>

void	ft_strtok_reset(void)
{
	static int	i = 0;

	i = 0;
}

char	*ft_strtok(const char *str, char separator)
{
	static int	i = 0;
	int			j;
	int			len;
	char		*token;

	if (i == (int)ft_strlen(str))
		return (NULL);
	while (str[i] && str[i] == separator)
		i++;
	len = 0;
	while (str[len + i] && str[len + i] != separator)
		len++;
	token = malloc((len + 1) * sizeof(char));
	if (!token)
		return (NULL);
	j = 0;
	while ((str[i]) != separator && str[i] != '\0')
		token[j++] = str[i++];
	token[j] = '\0';
	return (token);
}

// char **tokenize(char *cmd)
// {
//     char **tokens;

//     tokens = ft_split(cmd, ' ');
//     return (tokens);
// }