#include "../inc/minishell.h"

void shell_loop()
{
 	char *line;
//	char **args = NULL;

    line = NULL;
  	while (1)
    {
  		write(1, "$> ", 3);
    	line = get_next_line(0);
		args = split_line(line);
        free(line);
    }
}

int main(int ac, char **av, char **envp)
{
  	(void) envp;
  	(void) av;
    if (ac != 1)
    {
      printf("valid exec.: ./minishell");
      return (0);
    }
    shell_loop();
	return (0);
}


