#include <signal.h>
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include <stdio.h>

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		cenv.exit_status = 1;
	}
}

void	sig_exec_handler(int sig)
{
	if (sig == SIGINT)
		cenv.exit_status = 130;
	else if (sig == SIGQUIT)
	{
		cenv.exit_status = 131;
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	}
}


void	init_exec_signals()
{
	struct sigaction sa;

	sa.__sigaction_u.__sa_handler = &sig_exec_handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_errors("sigaction SIGINT not initialized: ");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		handle_errors("sigaction SIGQUIT not initialize: ");
}

void	init_signals(void)
{
	// struct sigaction	sa;

	// sa.__sigaction_u.__sa_handler = &sig_handler;
	// if (sigaction(SIGINT, &sa, NULL) == -1)

	// 	handle_errors("sigaction SIGINT not initialized: ");
	signal(SIGINT, &sig_handler);
	signal(SIGQUIT, SIG_IGN);
	// if (sigaction(SIGQUIT, &sa, NULL) == -1)
	// 	handle_errors("sigaction SIGQUIT not initialize: ");
}