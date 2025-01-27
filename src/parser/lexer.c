#include "../../inc/minishell.h"

char	*ft_strtok(const char *str, char separator)
{
    static int i = 0;
    int j;
    int len;
    char *token;

    if (i == ft_strlen(str))
        return (NULL);
    while(str[i] && str[i] == separator)
        i++;
    len = 0;
    while (str[len + i] && str[len + i] != separator)
        len++;
    token = malloc((len + 1) * sizeof(char));
    if (!token)
        return (NULL);
    j = 0;
    while ((str[i]) != separator)
        token[j++] = str[i++];
    token[j] = '\0';
    return (token);
}

// int main()
// {
//     const char *str = "hello world";
//     char *token = ft_strtok(str, ' ');
//     EXPECT_STREQ(token, "hello");
//     token = ft_strtok(NULL, ' ');
// }

// char **tokenize(char *cmd)
// {
//     char **tokens;

//     tokens = ft_split(cmd, ' ');
//     return (tokens);
// }