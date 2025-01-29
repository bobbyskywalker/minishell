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

// undefined behaviour for closing double quote with single quote & vice versa
char	*ft_strtok(const char *str, char separator)
{
	static int	i = 0;
	int			j;
	int			len;
	char		*token;
    bool         in_quote;

    in_quote = false;
	if (!str || !str[0] || i == (int)ft_strlen(str))
    {
        i = 0;
		return (NULL);
    }
	while (str[i] && str[i] == separator)
		i++;
    if ((str[i] == '\'' || str[i] == '\"') && str[i - 1] == ' ')
        in_quote = true;
    len = 0;
	while (str[len + i] && ((str[len + i] != separator && in_quote == false) 
        || ((str[len + i] == '\'') || ((str[len + i] == '\"') && in_quote))))
		len++;
	token = malloc((len + 1) * sizeof(char));
	if (!token)
		return (NULL);
	j = 0;
    while (str[i] && ((str[i] != separator && in_quote == false) 
        || ((str[i] == '\'') || ((str[i] == '\"') && in_quote))))
		token[j++] = str[i++];
	token[j] = '\0';
	return (token);
}

// int main()
// {
//     char *cmd = "echo 123\"super\"";
//     char *token = ft_strtok(cmd, ' ');
//     printf("%s\n", token);
//     free(token);
//     return (0);
// }

// char **tokenize(char *cmd)
// {
//     char **tokens;

//     tokens = ft_split(cmd, ' ');
//     return (tokens);
// }