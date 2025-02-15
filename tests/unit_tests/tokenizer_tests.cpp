#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>
#include <string>

extern "C"
{
    #include "../../inc/minishell.h"
}

TEST(TokenizerTest, ValidSimpleTokens)
{
    char *str = "hello world   welcome";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "hello");
    EXPECT_STREQ(tokens[1], "world");
    EXPECT_STREQ(tokens[2], "welcome");
    EXPECT_EQ(tokens[3], nullptr);
}

TEST(TokenizerTest, EmptyTokens)
{
    char *str = "";
    char **tokens = tokenize(str);
    EXPECT_EQ(tokens, nullptr);
}

TEST(TokenizerTest, NullStr)
{
    char **tokens = tokenize(nullptr);
    EXPECT_EQ(tokens, nullptr);
}

TEST(TokenizerTest, SimpleShellLikeTokens)
{
    char *str = "echo 123\"   super\"";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "echo");
    EXPECT_STREQ(tokens[1], "123   super");
    EXPECT_STREQ(tokens[2], NULL);
}

TEST(TokenizerTest, AdvShellLikeTokens)
{
    char *str = "echo 123\"   super\" '|' grep '1' | wc\"-l\"";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "echo");
    EXPECT_STREQ(tokens[1], "123   super");
    EXPECT_STREQ(tokens[2], "\'|\'");
    EXPECT_STREQ(tokens[3], "grep");
    EXPECT_STREQ(tokens[4], "\'1\'");
    EXPECT_STREQ(tokens[5], "|");
    EXPECT_STREQ(tokens[6], "wc-l");
    EXPECT_EQ(tokens[7], nullptr);
}

TEST(TokenizerTest, AdvShellLikeTokens2)
{
    char *str = "echo \"123\"\"   super          \" '|||||' grep '1 0 1 2 3' | wc\"-lspwdcwks\"";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "echo");
    EXPECT_STREQ(tokens[1], "\"123\"");
    EXPECT_STREQ(tokens[2], "\"   super          \"");
    EXPECT_STREQ(tokens[3], "\'|||||\'");
    EXPECT_STREQ(tokens[4], "grep");
    EXPECT_STREQ(tokens[5], "\'1 0 1 2 3\'");
    EXPECT_STREQ(tokens[6], "|");
    EXPECT_STREQ(tokens[7], "wc-lspwdcwks");
    EXPECT_EQ(tokens[8], nullptr);
}

TEST(TokenizerTest, UnmatchedQuotes) {
    char *str = "\"echo \" \'\"hello\"\'";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "\"echo \"");
    EXPECT_STREQ(tokens[1], "\'\"hello\"\'");
}

TEST(TokenizerTest, SpecialCharacters) {
    char *str = "ls && echo 'done'";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "ls");
    EXPECT_STREQ(tokens[1], "&&");
    EXPECT_STREQ(tokens[2], "echo");
    EXPECT_STREQ(tokens[3], "'done'");
    EXPECT_EQ(tokens[4], nullptr);
}

TEST(TokenizerTest, RedirectionsSimple)
{
    char *str = "cat < input.txt > output.txt";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "cat");
    EXPECT_STREQ(tokens[1], "<");
    EXPECT_STREQ(tokens[2], "input.txt");
    EXPECT_STREQ(tokens[3], ">");
    EXPECT_STREQ(tokens[4], "output.txt");
    EXPECT_EQ(tokens[5], nullptr);
}

TEST(TokenizerTest, RedirectionsAppendAndHeredoc)
{
    char *str = "grep 'error' >> log.txt";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "grep");
    EXPECT_STREQ(tokens[1], "'error'");
    EXPECT_STREQ(tokens[2], ">>");
    EXPECT_STREQ(tokens[3], "log.txt");
    EXPECT_EQ(tokens[4], nullptr);
}

TEST(TokenizerTest, HeredocUsage)
{
    char *str = "cat << EOF";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "cat");
    EXPECT_STREQ(tokens[1], "<<");
    EXPECT_STREQ(tokens[2], "EOF");
    EXPECT_EQ(tokens[3], nullptr);
}

TEST(TokenizerTest, ComplexRedirectionsAndPipes)
{
    char *str = "grep 'error' < input.log | sort | uniq > output.log";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "grep");
    EXPECT_STREQ(tokens[1], "'error'");
    EXPECT_STREQ(tokens[2], "<");
    EXPECT_STREQ(tokens[3], "input.log");
    EXPECT_STREQ(tokens[4], "|");
    EXPECT_STREQ(tokens[5], "sort");
    EXPECT_STREQ(tokens[6], "|");
    EXPECT_STREQ(tokens[7], "uniq");
    EXPECT_STREQ(tokens[8], ">");
    EXPECT_STREQ(tokens[9], "output.log");
    EXPECT_EQ(tokens[10], nullptr);
}

TEST(TokenizerTest, MixedQuotesAndRedirections)
{
    char *str = "echo \"hello world\" > \"my file.txt\"";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "echo");
    EXPECT_STREQ(tokens[1], "\"hello world\"");
    EXPECT_STREQ(tokens[2], ">");
    EXPECT_STREQ(tokens[3], "\"my file.txt\"");
    EXPECT_EQ(tokens[4], nullptr);
}

TEST(TokenizerTest, ComplexShellCommand)
{
    char *str = "ls -l | grep 'Jan' > output.txt && echo \"Done\"";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "ls");
    EXPECT_STREQ(tokens[1], "-l");
    EXPECT_STREQ(tokens[2], "|");
    EXPECT_STREQ(tokens[3], "grep");
    EXPECT_STREQ(tokens[4], "'Jan'");
    EXPECT_STREQ(tokens[5], ">");
    EXPECT_STREQ(tokens[6], "output.txt");
    EXPECT_STREQ(tokens[7], "&&");
    EXPECT_STREQ(tokens[8], "echo");
    EXPECT_STREQ(tokens[9], "\"Done\"");
    EXPECT_EQ(tokens[10], nullptr);
}

TEST(TokenizerTest, MultipleRedirectionsAndWhitespace)
{
    char *str = "cat   <    input.txt    >   output.txt  ";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "cat");
    EXPECT_STREQ(tokens[1], "<");
    EXPECT_STREQ(tokens[2], "input.txt");
    EXPECT_STREQ(tokens[3], ">");
    EXPECT_STREQ(tokens[4], "output.txt");
    EXPECT_EQ(tokens[5], nullptr);
}

TEST(TokenizerTest, ConsecutiveRedirections)
{
    char *str = "command >>> file.txt << EOF";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "command");
    EXPECT_STREQ(tokens[1], ">>>");
    EXPECT_STREQ(tokens[2], "file.txt");
    EXPECT_STREQ(tokens[3], "<<");
    EXPECT_STREQ(tokens[4], "EOF");
    EXPECT_EQ(tokens[5], nullptr);
}

TEST(TokenizerTest, MixedQuotesWithRedirections)
{
    char *str = "echo 'Hello > world' > \"my output.txt\"";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "echo");
    EXPECT_STREQ(tokens[1], "'Hello > world'");
    EXPECT_STREQ(tokens[2], ">");
    EXPECT_STREQ(tokens[3], "\"my output.txt\"");
    EXPECT_EQ(tokens[4], nullptr);
}

TEST(TokenizerTest, MultiplePipesWithRedirections)
{
    char *str = "ls -l | grep 'txt' | sort > sorted_list.txt";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "ls");
    EXPECT_STREQ(tokens[1], "-l");
    EXPECT_STREQ(tokens[2], "|");
    EXPECT_STREQ(tokens[3], "grep");
    EXPECT_STREQ(tokens[4], "'txt'");
    EXPECT_STREQ(tokens[5], "|");
    EXPECT_STREQ(tokens[6], "sort");
    EXPECT_STREQ(tokens[7], ">");
    EXPECT_STREQ(tokens[8], "sorted_list.txt");
    EXPECT_EQ(tokens[9], nullptr);
}

TEST(TokenizerTest, RedirectionsAndLogicalOperators)
{
    char *str = "cmd < input.txt && grep 'hello' > output.txt || echo 'fail'";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "cmd");
    EXPECT_STREQ(tokens[1], "<");
    EXPECT_STREQ(tokens[2], "input.txt");
    EXPECT_STREQ(tokens[3], "&&");
    EXPECT_STREQ(tokens[4], "grep");
    EXPECT_STREQ(tokens[5], "'hello'");
    EXPECT_STREQ(tokens[6], ">");
    EXPECT_STREQ(tokens[7], "output.txt");
    EXPECT_STREQ(tokens[8], "||");
    EXPECT_STREQ(tokens[9], "echo");
    EXPECT_STREQ(tokens[10], "'fail'");
    EXPECT_EQ(tokens[11], nullptr);
}

TEST(TokenizerTest, SpaceBeforePipe)
{
    char *str = "   ls  -l   |  grep 'test' ";
    char **tokens = tokenize(str);
    EXPECT_STREQ(tokens[0], "ls");
    EXPECT_STREQ(tokens[1], "-l");
    EXPECT_STREQ(tokens[2], "|");
    EXPECT_STREQ(tokens[3], "grep");
    EXPECT_STREQ(tokens[4], "'test'");
    EXPECT_EQ(tokens[5], nullptr);
}

