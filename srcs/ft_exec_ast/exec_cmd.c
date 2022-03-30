#include "ast.h"
#include "minishell.h"
#include "builtins.h"
#include <stdio.h>

char	*find_path(char *f)
{
	int		i;
	char	*fpath;
	char	**paths;
	char	*newf;

	newf = ft_strjoin("/", f); //leaks detected maybe !
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	while (paths[i])
	{
		if (!fpath)
			free(fpath);
		fpath = ft_strjoin(paths[i], newf);
		if (access(fpath, F_OK) == 0)
		{
			free(newf);
			return (fpath);
		}
		i++;
	}
	free(fpath);
	free(newf);
	if (access(f, F_OK) == 0)
		return (f);
	return (NULL);
}

void	exec_cmd(t_cmd node, int *fd)
{
	int	pid_id;
	int	status;
	int	execve_ret;
	char	*tmp;

	//check si builtin
	if (get_builtin(node.cmd_name) != bi_none)
		return (exec_builtin(node, fd));
	//check si la fonction existe, si oui on peux fork, si non, terminé.
	tmp = node.cmd_name;
	node.cmd_name = find_path(node.cmd_name);
	if (!node.cmd_name)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(tmp, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		cenv.exit_status = 127;
		return ;
	}
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
		if (fd[1] > 0)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		}
		if (fd[0] > 0)
		{
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		}
		execve_ret = execve(node.cmd_name, node.cmd_arg, cenv.env);
		//ici free ce qu'on peu, on ne doit jamais arriver ici en vrai
		exit(execve_ret);
	}
	else
	{
		waitpid(pid_id, &status, 0);
		cenv.exit_status = WEXITSTATUS(status);
		if (fd[1] > 0)
			close(fd[1]);
		if (fd[0] > 0)
			close(fd[0]);
	}
	return ;
}