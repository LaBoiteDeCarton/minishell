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

void	sig_exec_father_handler(int sig)
{
	if (sig == SIGINT)
		cenv.exit_status = 130;
	else if (sig == SIGQUIT)
	{
		cenv.exit_status = 131;
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	}
}

void	init_heredoc_signal()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	init_exec_father_signal()
{
	struct sigaction sa;

	sa.__sigaction_u.__sa_handler = &sig_exec_father_handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_errors("sigaction SIGINT not initialize: ");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		handle_errors("sigaction SIGQUIT not initialize: ");
}

void	init_exec_children_signals()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	init_signals(void)
{
	struct sigaction	sa;

	sa.__sigaction_u.__sa_handler = &sig_handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_errors("sigaction SIGINT not initialized: ");
	signal(SIGQUIT, SIG_IGN);
	// if (sigaction(SIGQUIT, &sa, NULL) == -1)
	// 	handle_errors("sigaction SIGQUIT not initialize: ");
}