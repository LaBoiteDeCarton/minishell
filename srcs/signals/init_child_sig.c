/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_child_sig.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:46:00 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:46:31 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"
#include <termios.h>

static void	echoctl_on(void)
{
	struct termios		tm;

	if (tcgetattr(STDIN_FILENO, &tm) == -1)
		return (handle_errors("tcgetattr"));
	tm.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tm) == -1)
		return (handle_errors("tcsetattr"));
}

void	init_child_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (isatty(STDIN_FILENO))
		echoctl_on();
}
