/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:18 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:19 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
#include "builtins.h"
#include <sys/stat.h>

static int	check_if_directory(char *path)
{
	struct stat	fs;

	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		g_cenv.exit_status = 127;
		return (1);
	}
	if (stat(path, &fs))
	{
		handle_errors("stat");
		return (1);
	}
	if (S_ISDIR(fs.st_mode))
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		g_cenv.exit_status = 126;
		return (1);
	}
	return (0);
}

static void	cmd_child(t_cmd *node, int *fd)
{
	int	execve_ret;

	init_child_sig();
	if (fd[0] != -1 && (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1))
		handle_errors("Command");
	if (fd[1] != -1 && (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[1]) == -1))
		handle_errors("Command");
	execve_ret = execve(node->cmd_name, node->cmd_arg, g_cenv.env);
	handle_errors("execve");
	free_chartab(g_cenv.env);
	free_var(g_cenv.var);
	exit(execve_ret);
}

static void	fork_cmd(t_cmd *node, int *fd)
{
	pid_t	pid_id;
	int		status;

	pid_id = fork();
	if (pid_id == -1)
	{
		handle_errors("fork");
		g_cenv.exit_status = 130;
		return ;
	}
	else if (pid_id == 0)
		cmd_child(node, fd);
	init_father_sig();
	waitpid(pid_id, &status, 0);
	if (!WIFSIGNALED(status))
		g_cenv.exit_status = WEXITSTATUS(status);
}

void	exec_cmd(t_cmd *node, int *fd)
{
	expande_commande(node);
	if (!node->cmd_name)
	{
		g_cenv.exit_status = 0;
		return ;
	}
	if (get_builtin(node->cmd_name) != bi_none)
		return (exec_builtin(*node, fd));
	node->cmd_name = find_path(node->cmd_name);
	if (!node->cmd_name)
	{
		node->cmd_name = node->cmd_arg[0];
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(node->cmd_name, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		g_cenv.exit_status = 127;
		return ;
	}
	node->cmd_arg[0] = node->cmd_name;
	if (check_if_directory(node->cmd_name))
		return ;
	fork_cmd(node, fd);
}
