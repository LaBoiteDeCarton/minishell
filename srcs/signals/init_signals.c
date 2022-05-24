/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:46:47 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:46:49 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"
#include <termios.h>

static void	echoctl_off(void)
{
	struct termios		tm;

	if (tcgetattr(STDIN_FILENO, &tm) == -1)
		return (handle_errors("tcgetattr"));
	tm.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tm) == -1)
		return (handle_errors("tcsetattr"));
}

static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_cenv.exit_status = 1;
	}
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
