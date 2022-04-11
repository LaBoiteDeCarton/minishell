#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main(int ac, char **av, char **env)
{
	char *str;
	(void)ac;
	(void)av;
	int i;
	struct termios termios_p;
	struct termios raw;
	
	printf("PATH : %s\n", getenv("PATH"));
	i = 0;
	while (strncmp(env[i], "PATH", 4))
		i++;
	strcat(env[i], ":/unknown");
	// printf("%s\n", env[i]);
	printf("PATH : %s\n", getenv("PATH"));
	tcgetattr(STDIN_FILENO, &termios_p);
	raw = termios_p;
	raw.c_iflag = VEOL;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
	ioctl(STDIN_FILENO, FIONCLEX);
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			int file;

			file = open("file", O_RDONLY);
			printf("fd : %d\n", file);
			str = readline("minishell>");
			if (!str)
				printf("EOF received\n");
			else
			{
				if (!strcmp(str, "END"))
					break;
				add_history(str);
				clear_history();
				free(str);
			}
		}
		else
		{
			printf("FALSE\n");
			break ;
		}
	}
	return (0);
}