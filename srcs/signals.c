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
}

void	init_signals(void)
{
	// struct sigaction	sa;
	
	// sa.__sigaction_u.__sa_handler = &sigint_handler;
	// sigaction(SIGINT, &sa, NULL);
}