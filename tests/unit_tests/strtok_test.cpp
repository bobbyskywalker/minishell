#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>
#include <string>

extern "C"
{
    #include "../../inc/minishell.h"
}

// TEST(StrtokTest, ValidStrtok)
// {
//     const char *str = "hello world   welcome";
//     char *token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "hello");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "world");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "welcome");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_EQ(token, nullptr);
// }

// TEST(StrtokTest, EmptyStrtok)
// {
//     // ft_strtok_reset();
//     const char *str = "";
//     char *token = ft_strtok(str, ' ');
//     EXPECT_EQ(token, nullptr);
// }

// TEST(StrtokTest, NullStrtok)
// {
//     // ft_strtok_reset();
//     char *token = ft_strtok(nullptr, ' ');
//     EXPECT_EQ(token, nullptr);
// }

TEST(StrtokTest, SimpleShellLikeStrtok)
{
    char *str = "echo 123\"   super\"";
    // ft_strtok_reset();
    char *token = ft_strtok(str, ' ');
    EXPECT_STREQ(token, "echo");
    free(token);
    token = ft_strtok(str, ' ');
    EXPECT_STREQ(token, "123\"   super\"");
    free(token);
    token = ft_strtok(str, ' ');
    EXPECT_STREQ(token, NULL);
    free(token);
}

// TEST(StrtokTest, ShellLikeStrtok)
// {
//     char *str = "echo 123\"   super\" '|' grep '1' | wc\"-l\"";
//     // ft_strtok_reset();
//     char *token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "echo");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "123\"   super\"");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "\'|\'");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "grep");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "\'1\'");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "|");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "wc\"-l\"");
//     free(token);
//     token = ft_strtok(str, ' ');
//     EXPECT_EQ(token, nullptr);
// }