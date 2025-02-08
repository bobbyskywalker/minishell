#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>
#include <string>

extern "C"
{
    #include "../../inc/minishell.h"
}

TEST(AST, SimpleCommandNoPipes)
{
    char *input = "echo -n";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);
    EXPECT_EQ(ast->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);

    input = "ls -l";
    tokens = tokenize(input);
    ast = build_ast(tokens);
    EXPECT_EQ(ast->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);

    input = "wc -l -r -t -a -b -q -d -w";
    tokens = tokenize(input);
    ast = build_ast(tokens);
    EXPECT_EQ(ast->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}


TEST(AST, SinglePipes)
{
    char *input = "ls | grep coding";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);
    EXPECT_EQ(ast->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);


    input = "ls -lra | wc -lueir";
    tokens = tokenize(input);
    ast = build_ast(tokens);
    EXPECT_EQ(ast->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);


    input = "echo -n dupa | grep -s d";
    tokens = tokenize(input);
    ast = build_ast(tokens);
    EXPECT_EQ(ast->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);
}

TEST(AST, DoublePipes)
{
    char *input = "ls | grep coding | wc -l";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);

    EXPECT_EQ(ast->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->type, PIPE_NODE);
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->left_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->right_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}

TEST(AST, MultiplePipes)
{
    char *input = "ls | grep coding | wc -l";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);

    EXPECT_EQ(ast->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->type, PIPE_NODE);
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->left_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->right_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);

    input = "echo \"123\" super\"poggers\" | grep -w \"super\" | wc -l -p -r";
    tokens = tokenize(input);
    ast = build_ast(tokens);

    EXPECT_EQ(ast->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->type, PIPE_NODE);
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->left_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->right_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}

// TODO: heredoc parsing
TEST(AST, SimpleRedirection)
{
    char *input = "ls > file.txt";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);
    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, OUTPUT_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "file.txt");
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);

    input = "ls >> file.txt";
    tokens = tokenize(input);
    ast = build_ast(tokens);
    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, APPEND_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "file.txt");
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);

    input = "ls < file.txt";
    tokens = tokenize(input);
    ast = build_ast(tokens);
    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, INPUT_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "file.txt");
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}

TEST(AST, MultipleRedirections)
{
    char *input = "ls > file.txt >> file2.txt";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);
    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, APPEND_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "file2.txt");
    EXPECT_EQ(ast->left_child->type, REDIRECT_NODE);
    EXPECT_EQ(ast->left_child->redirect->type, OUTPUT_REDIRECT);
    EXPECT_STREQ(ast->left_child->redirect->filename, "file.txt");
    EXPECT_EQ(ast->left_child->left_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}

TEST(AST, DoubleRedirections)
{
    char *input = "ls >> file.txt >> file2.txt";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);
    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, APPEND_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "file2.txt");
    EXPECT_EQ(ast->left_child->type, REDIRECT_NODE);
    EXPECT_EQ(ast->left_child->redirect->type, APPEND_REDIRECT);
    EXPECT_STREQ(ast->left_child->redirect->filename, "file.txt");
    EXPECT_EQ(ast->left_child->left_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}

TEST(AST, MultipleRedirectionsAndPipes)
{
    char *input = "ls | grep coding | wc -l > file.txt >> file2.txt";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);
    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, APPEND_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "file2.txt");
    EXPECT_EQ(ast->left_child->type, REDIRECT_NODE);
    EXPECT_EQ(ast->left_child->redirect->type, OUTPUT_REDIRECT);
    EXPECT_STREQ(ast->left_child->redirect->filename, "file.txt");
    EXPECT_EQ(ast->left_child->left_child->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->left_child->right_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->left_child->left_child->type, PIPE_NODE);
    EXPECT_EQ(ast->left_child->left_child->left_child->right_child->type, COMMAND_NODE);
    EXPECT_EQ(ast->left_child->left_child->left_child->left_child->type, COMMAND_NODE);
    ft_arr2d_free(tokens);
    free_ast(ast);
}

TEST(AST, MultipleRedirectionsAndPipes2)
{
    char *input = "cat input.txt | grep \"error\" | sort > sorted_errors.txt >> error_log.txt";
    char **tokens = tokenize(input);
    t_ast_node *ast = build_ast(tokens);

    EXPECT_EQ(ast->type, REDIRECT_NODE);
    EXPECT_EQ(ast->redirect->type, APPEND_REDIRECT);
    EXPECT_STREQ(ast->redirect->filename, "error_log.txt");

    EXPECT_EQ(ast->left_child->type, REDIRECT_NODE);
    EXPECT_EQ(ast->left_child->redirect->type, OUTPUT_REDIRECT);
    EXPECT_STREQ(ast->left_child->redirect->filename, "sorted_errors.txt"); // redirect 2
    
    EXPECT_EQ(ast->left_child->left_child->type, PIPE_NODE); // grep error | sort
    EXPECT_EQ(ast->left_child->left_child->right_child->type, COMMAND_NODE); // sort
    EXPECT_EQ(ast->left_child->left_child->left_child->type, PIPE_NODE); // cat | grep
    EXPECT_EQ(ast->left_child->left_child->left_child->right_child->type, COMMAND_NODE); // grep
    EXPECT_EQ(ast->left_child->left_child->left_child->left_child->type, COMMAND_NODE); // cat
    ft_arr2d_free(tokens);
    free_ast(ast);
}