/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarbacz <agarbacz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:08:54 by agarbacz          #+#    #+#             */
/*   Updated: 2025/01/28 18:08:55 by agarbacz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast_node *create_cmd_node(char **args)
{
    t_ast_node *node;
    t_command   *cmd;

    node = malloc(sizeof(t_ast_node));
    if (!node)
        return (NULL);
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = args;
    node->type = COMMAND_NODE;
    node->left_child = NULL;
    node->right_child = NULL;
    node->command = cmd;
    return (node);
}

// operator node creator
// left child: left-of-operator cmd
// right child: right-of-operator cmd
t_ast_node *create_operator_node(t_node_type type, t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node;
    
    node = malloc (sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = type;
    node->left_child = left;
    node->right_child = right;
    node->command = NULL;
    return (node);
}

// frees the syntax tree recursively
void free_AST(t_ast_node *root)
{
    if (!root)
        return;
    free_AST(root->left_child);
    free_AST(root->right_child);
    if (root->type == COMMAND_NODE && root->command)
        free(root->command);
    free(root);
}