#include "ast.h"
#include "minishell.h"
#include "builtins.h"
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

static char	*find_path(char *f)
{
	int		i;
	char	*fpath;
	char	**paths;
	char	*newf;

	if (!ft_strlen(f))
		return (NULL);
	newf = get_value("PATH");
	if (newf && *newf != '\0')
	{
		paths = ft_split(newf, ':');
		if (!paths)
		{
			handle_errors(f);
			return (f);
		}
	}
	else
		paths = NULL;
	newf = ft_strjoin("/", f);
	i = 0;
	fpath = NULL;
	while (paths && paths[i])
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
	if (paths)
		free_chartab(paths);
	if (fpath)
		free(fpath);
	free(newf);
	if (access(f, F_OK) == 0)
		return (f);
	return (NULL);
}

static int	check_if_directory(char *path)
{
	struct stat fs;

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
		cenv.exit_status = 126;
		return (1);
	}
	return (0);
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
	if (check_if_directory(node->cmd_name))
		return ;
	pid_id = fork();
	if (pid_id == -1)
	{
		handle_errors("fork");
		cenv.exit_status = 130;
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
		handle_errors("execve");
		free_chartab(cenv.env);
		free_var(cenv.var);
		exit(execve_ret);
	}
	init_father_sig();
	waitpid(pid_id, &status, 0);
	if (!WIFSIGNALED(status))
		cenv.exit_status = WEXITSTATUS(status);
	return ;
}