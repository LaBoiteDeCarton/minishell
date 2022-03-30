#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static char	*ft_readline(char *prompt_msg)
{
	char	*read_buffer;

	if (isatty(STDIN_FILENO))
		read_buffer = readline(prompt_msg);
	else
		get_next_line(STDIN_FILENO, &read_buffer);
	return (read_buffer);
}

static void	interactive_msh()
{
	char	*prompt_msg;
	//char	*readline_buffer;

	while (1)
	{
		prompt_msg = get_prompt();
		rl_line_buffer = ft_readline(prompt_msg);
		free(prompt_msg);
		if (rl_line_buffer != NULL && ft_strncmp(rl_line_buffer, "", 1))
			add_history(rl_line_buffer);
		ft_system(rl_line_buffer);
		free(rl_line_buffer);
	}
}

static void	passive_msh()
{
	char	*read_stdin;

	read_stdin = ft_readline(NULL);
	ft_system(read_stdin);
	free(read_stdin);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	cenv.env = env;
	cenv.exit_status = 0;
	cenv.var = NULL;

	init_signals();
	if (isatty(STDIN_FILENO))
		interactive_msh();
	else
		passive_msh();
	return (cenv.exit_status);
}
