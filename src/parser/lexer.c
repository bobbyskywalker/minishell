/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:51 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/14 13:29:18 by agarbacz         ###   ########.fr       */
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

// tokenizer loop, fills the array of tokens
void	tokenize_loop(char *source, char **tokens, int end_flag, int i)
{
	while (1)
	{
		tokens[i] = ft_calloc((ft_strlen(source) + 1), sizeof(char));
		if (!tokens[i])
			return ;
		source = get_token(source, tokens[i], &end_flag);
		if (tokens[i] == NULL || !tokens[i][0] || (!source && !end_flag))
		{
			free(tokens[i]);
			tokens[i] = NULL;
			break ;
		}
		if ((source || *source) || end_flag == 1)
		{
			if (end_flag == 1)
			{
				free(tokens[i]);
				tokens[i] = NULL;
				break ;
			}
			i++;
			tokens[i] = NULL;
		}
	}
}

// highest level lexer function
// creates an array of tokens filled in tokenize_loop()
char	**tokenize(char *source)
{
	char	**tokens;
	int		end_flag;

	if (!source || !*source)
		return (NULL);
	tokens = ft_calloc((ft_strlen(source) + 1), sizeof(char *));
	if (!tokens)
		return (NULL);
	end_flag = 0;
	tokenize_loop(source, tokens, end_flag, 0);
	return (tokens);
}

// utility to extract the next token
char	*get_next_token(char ***tokens)
{
	if (!(**tokens))
		return (NULL);
	return (*(*tokens)++);
}
