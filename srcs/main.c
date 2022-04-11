#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


static void	interactive_msh()
{
	char	*readline_buffer;

	while (1)
	{
		readline_buffer = NULL;
		readline_buffer = ft_readline("msh-0.1> ");
		if (!readline_buffer)
			msh_exit();
		if (*readline_buffer)
			add_history(readline_buffer);
		ft_system(readline_buffer);
		free(readline_buffer);
		init_signals();
	}
}

static void	passive_msh()
{
	char	*read_stdin;

	read_stdin = NULL;
	read_stdin = ft_readline(NULL);
	while (read_stdin)
	{
		ft_system(read_stdin);
		free(read_stdin);
		read_stdin = NULL;
		read_stdin = ft_readline(NULL);
		init_signals();
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	set_env(env);
	cenv.exit_status = 0;
	cenv.var = NULL;
	init_signals();
	if (isatty(STDIN_FILENO))
		interactive_msh();
	else
		passive_msh();
	exit(cenv.exit_status);
}
