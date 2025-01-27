#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>
#include <string>

extern "C"
{
    #include "../../inc/minishell.h"
}

TEST(StrtokTest, ValidStrtok)
{
    const char *str = "hello world   welcome";
    char *token = ft_strtok(str, ' ');
    EXPECT_STREQ(token, "hello");
    free(token);
    token = ft_strtok(str, ' ');
    EXPECT_STREQ(token, "world");
    free(token);
    token = ft_strtok(str, ' ');
    EXPECT_STREQ(token, "welcome");
    token = ft_strtok(str, ' ');
    EXPECT_EQ(token, nullptr);
}