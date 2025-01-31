/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:24:38 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/31 13:07:11 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

t_machine_state	handle_token_state(char **source, char *token, size_t *i,
		int *end_flag)
{
	if (**source == ' ' || **source == '\0' || **source == '\''
		|| **source == '"')
	{
		token[*i] = 0;
		if (**source == '\0')
			*end_flag = 1;
		return (END);
	}
	else
	{
		token[*i] = **source;
		(*i)++;
		(*source)++;
		return (TOKEN);
	}
}
