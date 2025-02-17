/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:24:38 by agarbacz          #+#    #+#             */
/*   Updated: 2025/02/17 10:20:47 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// processes token extraction when in START state
void	handle_start_state(char **source, char *token, size_t *i,
		t_machine_state *state)
{
	if (**source == '\'')
	{
		*state = SINGLEQUOTES;
		token[*i] = **source;
		(*i)++;
		(*source)++;
	}
	else if (**source == '"')
	{
		*state = DOUBLEQUOTES;
		token[*i] = **source;
		(*i)++;
		(*source)++;
	}
	else if (**source == ' ')
		(*source)++;
	else
	{
		*state = TOKEN;
		token[*i] = **source;
		(*i)++;
		(*source)++;
	}
}

// processes token extraction when in SINGLEQUOTES state
void	handle_single_quotes(char **source, char *token, size_t *i,
		t_machine_state *state)
{
	token[*i] = **source;
	(*i)++;
	(*source)++;
	if (**source == '\'')
	{
		token[*i] = **source;
		(*i)++;
		(*source)++;
		token[*i] = 0;
		*state = END;
	}
}

// processes token extraction when in DOUBLEQUOTES state
void	handle_double_quotes(char **source, char *token, size_t *i,
		t_machine_state *state)
{
	token[*i] = **source;
	(*i)++;
	(*source)++;
	if (**source == '"')
	{
		token[*i] = **source;
		(*i)++;
		(*source)++;
		token[*i] = 0;
		*state = END;
	}
}

// a utility for TOKEN machine state parsing
static int	is_inside_quotes(char quote_char)
{
	static char	current_quote;

	current_quote = 0;
	if (quote_char == '"' || quote_char == '\'')
	{
		if (current_quote == 0)
			current_quote = quote_char;
		else if (current_quote == quote_char)
			current_quote = 0;
	}
	return (current_quote != 0);
}

// processes token extraction when in TOKEN state
t_machine_state	handle_token_state(char **source, char *token, size_t *i,
		int *end_flag)
{
	if (**source == '"' || **source == '\'')
		is_inside_quotes(**source);
	if ((**source == ' ' || **source == '\0') && !is_inside_quotes(0))
	{
		token[*i] = 0;
		if (**source == '\0')
			*end_flag = 1;
		return (END);
	}
	else
	{
		if (**source != '"' && **source != '\'')
		{
			token[*i] = **source;
			(*i)++;
		}
		(*source)++;
		return (TOKEN);
	}
}
