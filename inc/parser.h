#ifndef PARSER_H
# define PARSER_H

#include "minishell.h"

typedef enum e_machine_state
{
	START,
	SINGLEQUOTES,
	DOUBLEQUOTES,
	TOKEN,
	END
}						t_machine_state;

void	handle_start_state(char **source, char *token, size_t *i,
		t_machine_state *state);
void	handle_single_quotes(char **source, char *token, size_t *i,
		t_machine_state *state);
void	handle_double_quotes(char **source, char *token, size_t *i,
		t_machine_state *state);
void	handle_token_state(char **source, char *token, size_t *i,
		t_machine_state *state, int *end_flag);

#endif 