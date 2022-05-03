#include <signal.h>
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include <stdio.h>
#include <termios.h>

void	echoctl_on(void)
{
	struct termios		tm;

	if (tcgetattr(STDIN_FILENO, &tm) == -1)
		return (handle_errors("tcgetattr"));
	tm.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tm) == -1)
		return (handle_errors("tcsetattr"));
}

void	echoctl_off(void)
{
	struct termios		tm;

	if (tcgetattr(STDIN_FILENO, &tm) == -1)
		return (handle_errors("tcgetattr"));
	tm.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tm) == -1)
		return (handle_errors("tcsetattr"));
}

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

void	init_heredoc_signal()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_father(int sig)
{
	if (sig == SIGINT)
	{
		cenv.exit_status = 130;
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	if (sig == SIGQUIT)
	{
		cenv.exit_status = 131;
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	}
}

void	init_father_sig()
{
	struct sigaction sa;

	sa.__sigaction_u.__sa_handler = &sig_father;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_errors("sigaction: ");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		handle_errors("sigaction: ");
}

void	init_child_sig()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (isatty(STDIN_FILENO))
		echoctl_on();
}

void	init_signals(void)
{
	struct sigaction	sa;

	sa.__sigaction_u.__sa_handler = &sig_handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_errors("sigaction: ");
	signal(SIGQUIT, SIG_IGN);
	if (isatty(STDIN_FILENO))
		echoctl_off();
}