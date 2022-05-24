/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_father_sig.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:46:35 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:46:36 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"

static void	sig_father(int sig)
{
	if (sig == SIGINT)
	{
		g_cenv.exit_status = 130;
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	if (sig == SIGQUIT)
	{
		g_cenv.exit_status = 131;
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	}
}

void	init_father_sig(void)
{
	struct sigaction	sa;

	sa.__sigaction_u.__sa_handler = &sig_father;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_errors("sigaction: ");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		handle_errors("sigaction: ");
}
