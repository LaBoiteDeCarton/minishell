/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:50:11 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:50:12 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

void	exec_builtin_env(t_cmd node, int *fd)
{
	char	**chartab;

	(void)node;
	if (fd[1] <= 0)
		fd[1] = dup(STDOUT_FILENO);
	if (fd[1] == -1)
		return (handle_errors("Env: "));
	chartab = g_cenv.env;
	while (*chartab)
	{
		if (ft_strchr(*chartab, '='))
			ft_putendl_fd(*chartab, fd[1]);
		chartab++;
	}
	g_cenv.exit_status = 0;
}
