#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>
#include <string>

extern "C"
{
    #include "../../inc/minishell.h"
}

TEST(AST, test1)
{
    // Test input: "ls -l | grep 'Jan' > output.txt"
    char *input[] = {"ls", "-l", "|", "grep", "Jan", ">", "output.txt", NULL};

    // Build the AST
    t_ast_node *ast = build_ast(input);

    // Check the root node is a PIPE_NODE
    EXPECT_EQ(ast->type, PIPE_NODE);

    // Left child of the pipe should be a COMMAND_NODE ("ls -l")
    EXPECT_EQ(ast->left_child->type, COMMAND_NODE);
    EXPECT_STREQ(ast->left_child->command->args[0], "ls");
    EXPECT_STREQ(ast->left_child->command->args[1], "-l");
    EXPECT_STREQ(ast->left_child->command->args[2], NULL);

    // Right child of the pipe should be a COMMAND_NODE ("grep Jan")
    EXPECT_EQ(ast->right_child->type, COMMAND_NODE);
    EXPECT_STREQ(ast->right_child->command->args[0], "grep");
    EXPECT_STREQ(ast->right_child->command->args[1], "Jan");
    EXPECT_STREQ(ast->right_child->command->args[2], NULL);

    // Right child has a redirection node ">"
    EXPECT_EQ(ast->right_child->left_child->type, REDIRECT_NODE);

    // The redirection type should be OUTPUT_REDIRECT (since it's ">" )
    EXPECT_EQ(ast->right_child->left_child->redirect->type, OUTPUT_REDIRECT);

    // The filename should be "output.txt"
    EXPECT_EQ(ft_strncmp(ast->right_child->left_child->redirect->filename, "output.txt", ft_strlen("output.txt")), 0);

    // Clean up the AST
    free_ast(ast);
}
