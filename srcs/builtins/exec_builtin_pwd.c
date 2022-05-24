/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:50:35 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:50:36 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

char	*ft_getcwd(void)
{
	if (!g_cenv.tcwd)
		g_cenv.tcwd = getcwd(NULL, 0);
	if (!g_cenv.tcwd)
		return (NULL);
	return (g_cenv.tcwd);
}

void	exec_builtin_pwd(t_cmd node, int *fd)
{
	char	*cwd;

	(void)node;
	if (fd[1] == -1)
		fd[1] = dup(STDOUT_FILENO);
	cwd = ft_getcwd();
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", STDERR_FILENO);
		ft_putstr_fd("getcwd: ", STDERR_FILENO);
		perror("cannot access parent directories");
		g_cenv.exit_status = 130;
		return ;
	}
	ft_putendl_fd(cwd, fd[1]);
	g_cenv.exit_status = 0;
}
