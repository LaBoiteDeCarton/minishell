#include <signal.h>
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include <stdio.h>

void	sigint_handler(int sig)
{
	(void)sig;

	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	signal(SIGINT, &sigint_handler);
}

void	init_signals(void)
{
	signal(SIGINT, &sigint_handler);
}