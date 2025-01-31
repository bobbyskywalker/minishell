/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:51 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 13:08:33 by agarbacz         ###   ########.fr       */
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
			state = handle_token_state(&source, token, &i, end_flag);
		else if (state == END)
			return (source);
	}
	return (source);
}

// returns token, why would you ask? beacuse it needs to be freed
// and norminette is a hoe
char	*tokenize_loop(char *source, char **tokens, int i, int end_flag)
{
	char	*token;

	while (1)
	{
		token = ft_calloc((ft_strlen(source) + 1), sizeof(char));
		if (!token)
			return (NULL);
		source = get_token(source, token, &end_flag);
		if (!token || !*token || (!source && !end_flag))
		{
			tokens[i] = NULL;
			break ;
		}
		if ((source || *source) || end_flag == 1)
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
	return (token);
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
	tokens = ft_calloc((ft_strlen(source) + 1), sizeof(char *));
	if (!tokens)
		return (NULL);
	end_flag = 0;
	token = tokenize_loop(source, tokens, i, end_flag);
	if (token)
		free(token);
	return (tokens);
}
