/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzackiew <jzackiew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:18:34 by jzackiew          #+#    #+#             */
/*   Updated: 2025/02/12 12:12:20 by jzackiew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include <stdbool.h>

typedef enum e_machine_state
{
	START,
	SINGLEQUOTES,
	DOUBLEQUOTES,
	TOKEN,
	END
}						t_machine_state;

// AST node type enumerations
typedef enum e_node_type
{
	COMMAND_NODE,
	PIPE_NODE,
	REDIRECT_NODE,
	// bonus (maybe) :))
	AND_NODE,
	OR_NODE
}						t_node_type;

typedef enum e_redirect_type
{
	INPUT_REDIRECT,
	OUTPUT_REDIRECT,
	APPEND_REDIRECT,
	HEREDOC
}						t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*filename;
	bool				is_heredoc;
	char				*limiter;
	bool				limiter_assigned;
}						t_redirect;

typedef struct s_command
{
	char				**args;
}						t_command;

typedef struct s_ast_node
{
	t_node_type			type;
	struct s_ast_node	*left_child;
	struct s_ast_node	*right_child;
	struct s_redirect	*redirect;
	t_command			*command;
}						t_ast_node;

typedef struct s_shell_data
{
	char				**env_vars;
	int					last_cmd_status;
	t_ast_node			*root;
}						t_shell_data;

// SECTION: lexer
char					**tokenize(char *source);
void					handle_start_state(char **source, char *token,
							size_t *i, t_machine_state *state);
void					handle_single_quotes(char **source, char *token,
							size_t *i, t_machine_state *state);
void					handle_double_quotes(char **source, char *token,
							size_t *i, t_machine_state *state);
t_machine_state			handle_token_state(char **source, char *token,
							size_t *i, int *end_flag);

// SECTION: AST library
char					*get_next_token(char ***tokens);
t_ast_node				*create_cmd_node(char **args);
t_ast_node				*create_operator_node(t_node_type type,
							t_ast_node *left, t_ast_node *right);
t_ast_node				*create_redirect_node(t_redirect_type type,
							char *filename, t_ast_node *child);
void					free_ast(t_ast_node *root);

// SECTION: parser
t_redirect_type			assign_redirection_type(char *token);
t_ast_node				*parse_cmd(char ***tokens);
t_ast_node				*parse_pipeline_part(char ***tokens);
t_ast_node				*parse_redirections_part(char ***tokens,
							t_ast_node *node);
t_ast_node				*build_ast(char **tokens);
void					print_ast(t_ast_node *node, int depth);

#endif