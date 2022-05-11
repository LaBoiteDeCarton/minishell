#include "ast.h"
#include "minishell.h"
#include "builtins.h"
#include <stdio.h>
#include <signal.h>

static char	*find_path(char *f)
{
	int		i;
	char	*fpath;
	char	**paths;
	char	*newf;

	newf = ft_strjoin("/", f);
	paths = ft_split(getenv("PATH"), ':'); //ft_getenv
	if (!paths)
	{
		free(newf);
		handle_errors(f);
		return (f);
	}
	i = 0;
	fpath = NULL;
	while (paths[i])
	{
		if (fpath)
			free(fpath);
		fpath = ft_strjoin(paths[i], newf);
		if (!fpath)
			handle_errors("Command: ");
		else if (access(fpath, F_OK) == 0)
		{
			free(f);
			free(newf);
			free_chartab(paths);
			return (fpath);
		}
		i++;
	}
	free_chartab(paths);
	free(fpath);
	free(newf);
	if (access(f, F_OK) == 0)
		return (f);
	return (NULL);
}

void	exec_cmd(t_cmd *node, int *fd)
{
	pid_t	pid_id;
	int		status;
	int		execve_ret;

	expande_commande(node);
	if (get_builtin(node->cmd_name) != bi_none)
		return (exec_builtin(*node, fd));
	node->cmd_name = find_path(node->cmd_name);
	if (!node->cmd_name)
	{
		node->cmd_name = node->cmd_arg[0];
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(node->cmd_name, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		cenv.exit_status = 127;
		return ;
	}
	node->cmd_arg[0] = node->cmd_name;
	// ATTENTION, si cmd_name vide, ne rien faire
	pid_id = fork();
	if (pid_id == -1)
	{
		write(2, "Unable to fork\n", 15);
		cenv.exit_status = 130; //ici trouver la valeur pour une erreur en interne
		return ;
	}
	else if (pid_id == 0)
	{
		init_child_sig();
		if (fd[0] != -1 && (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1)) //do_piping
			handle_errors("Command");
		if (fd[1] != -1 && (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[1]) == -1))
			handle_errors("Command");
		execve_ret = execve(node->cmd_name, node->cmd_arg, cenv.env);
		//ici free ce qu'on peu, on ne doit jamais arriver ici en vrai
		ft_putstr_fd("neverland\n", STDOUT_FILENO);
		handle_errors("execve");
		exit(execve_ret);
	}
	init_father_sig();
	waitpid(pid_id, &status, 0);
	if (!WIFSIGNALED(status))
		cenv.exit_status = WEXITSTATUS(status);
	return ;
}