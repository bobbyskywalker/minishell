/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:51 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/30 15:39:49 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// undefined behaviour for closing double quote with single quote & vice versa
// Initialized with start state START
char	*get_token(char *source, char *token, int *end_flag)
{
	size_t			i;
	t_machine_state	state;

	state = START;
	i = 0;
	if (*source == 0)
		return (NULL);
	while (*source != 0)
	{
		if (state == START)
			handle_start_state(&source, token, &i, &state);
		else if (state == SINGLEQUOTES)
			handle_single_quotes(&source, token, &i, &state);
		else if (state == DOUBLEQUOTES)
			handle_double_quotes(&source, token, &i, &state);
		else if (state == TOKEN)
			handle_token_state(&source, token, &i, &state, end_flag);
		else if (state == END)
			return (source);
	}
	return (source);
}

char	**tokenize(char *source)
{
	int		i;
	char	**tokens;
	char	*token;
	int		end_flag;

	if (!source || !*source)
		return (NULL);
	i = 0;
	tokens = malloc(sizeof(char *) * (ft_strlen(source) + 1));
	if (!tokens)
		return (NULL);
	end_flag = 0;
	while (1)
	{
		token = ft_calloc((ft_strlen(source) + 1), sizeof(char));
		if (!token)
			return (NULL);
		source = get_token(source, token, &end_flag);
		if (source == NULL && end_flag == 0)
			break ;
		if (source || end_flag == 1)
		{
			tokens[i++] = ft_strdup(token);
			free(token);
			if (end_flag == 1)
			{
				tokens[i] = NULL;
				break ;
			}
		}
	}
	if (token)
		free(token);
	tokens[i] = NULL;
	return (tokens);
}
// int	main(void)
// {
// 	char *source = "echo 123\"   super\" '|' grep '1' | wc\"-l\"";

// 	char **tokens = tokenize(source);
// 	(void)tokens;
// 	// for (int i = 0; i < 2; i++)
// 	// 	printf("%s", tokens[i]);
// 	return (347);
// }