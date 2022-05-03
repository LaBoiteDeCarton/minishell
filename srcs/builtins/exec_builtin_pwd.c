#include "builtins.h"
#include "minishell.h"

char	*ft_getcwd()
{
	if (!cenv.tcwd)
		cenv.tcwd = getcwd(NULL, 0);
	if (!cenv.tcwd)
		return (NULL);
	return (cenv.tcwd);
}

void	exec_builtin_pwd(t_cmd node, int *fd)
{
	(void)node;
	char	*cwd;

	if (fd[1] == -1)
		fd[1] = dup(STDOUT_FILENO);
	cwd = ft_getcwd();
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", STDERR_FILENO);
		ft_putstr_fd("getcwd: ", STDERR_FILENO);
		perror("cannot access parent directories");
		cenv.exit_status = 130;
		return ;
	}
	ft_putendl_fd(cwd, fd[1]);
	cenv.exit_status = 0;
}
