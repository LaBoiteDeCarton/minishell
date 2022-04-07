#include "minishell.h"

char	*ft_readline(char *prompt_msg)
{
	char	*read_buffer;

	read_buffer = NULL;
	if (isatty(STDIN_FILENO))
		read_buffer = readline(prompt_msg);
	else
		get_next_line(STDIN_FILENO, &read_buffer);
	return (read_buffer);
}