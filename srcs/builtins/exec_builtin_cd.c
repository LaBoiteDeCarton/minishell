/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:49:59 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:50:00 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"
#include <sys/stat.h>

/*
	BUILT-IN CD
	How it works :
		do chdir:
			if return (-1) - the directory doesn't exist
			if return (0) - we do the maj of the tcwd 
				(The Current Work Directory) value
	checker si path existe
		tel quel, puis accroché au pwd (getcwd)

	Si oui et OLDPWD existe mettre la variable env OLDPWD avec la valeur 
	du current PWD
	Si oui et PWD existe mettre la variable env PWD avec la valeur du 
	nouveau PWD


	!args[1] || ft_strequ(args[1], "~") 
	home unset error "cd: HOME not set"
*/

static void	maj_tcwd(char *path)
{
	char	*new_tcwd;

	new_tcwd = getcwd(NULL, 0);
	if (!new_tcwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", STDERR_FILENO);
		ft_putstr_fd("getcwd: ", STDERR_FILENO);
		perror("cannot access parent directories");
		path = ft_strjoin("/", path);
		new_tcwd = ft_strjoin(g_cenv.tcwd, path);
		if (path)
			free(path);
	}
	if (g_cenv.tcwd)
		free(g_cenv.tcwd);
	g_cenv.tcwd = new_tcwd;
}

static char	*find_home(void)
{
	char	*home;

	home = get_value("HOME");
	if (!home)
	{
		g_cenv.exit_status = 1;
		ft_putstr_fd("msh: cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (home);
}

void	exec_builtin_cd(t_cmd node, int *fd)
{
	char	*temp;
	char	*where_to_go;

	(void)fd;
	if (!node.cmd_arg[1])
		where_to_go = find_home();
	else
		where_to_go = node.cmd_arg[1];
	if (!where_to_go)
		return ;
	if (chdir(where_to_go) == -1)
	{
		g_cenv.exit_status = 1;
		handle_errors(where_to_go);
	}
	else
	{
		g_cenv.exit_status = 0;
		maj_tcwd(node.cmd_arg[1]);
		add_param("OLDPWD=$PWD");
		temp = ft_strjoin("PWD=", g_cenv.tcwd);
		add_param(temp);
		free(temp);
	}
}
