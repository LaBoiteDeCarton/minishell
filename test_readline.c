#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char **av, char **env)
{
	char *str;
	(void)ac;
	(void)av;
	int i;
	
	printf("PATH : %s\n", getenv("PATH"));
	i = 0;
	while (strncmp(env[i], "PATH", 4))
		i++;
	strcat(env[i], ":/unknown");
	// printf("%s\n", env[i]);
	printf("PATH : %s\n", getenv("PATH"));
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("TRUE\n");
			str = readline("minishell>");
			if (!strcmp(str, "END"))
				break;
			add_history(str);
			clear_history();
			free(str);
		}
		else
		{
			printf("FALSE\n");
			break ;
		}
	}
	return (0);
}