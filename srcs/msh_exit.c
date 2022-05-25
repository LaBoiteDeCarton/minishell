/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:37:23 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:37:24 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	msh_exit(void)
{
	free_chartab(g_cenv.env);
	free_var(g_cenv.var);
	rl_clear_history();
	if (g_cenv.ast)
		ft_lstclear(&g_cenv.ast, &free_ast);
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	system("leaks minishell");
	exit(g_cenv.exit_status);
}
