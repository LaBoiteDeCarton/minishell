#include "builtins.h"
#include "minishell.h"

void	exec_builtin_pwd(t_cmd node, int *fd)
{
	(void)node;
	char	*buf;

	if (fd[1] == -1)
		fd[1] = dup(STDOUT_FILENO);
	buf = (char *)malloc(sizeof(char) * 1000);
	if (!buf)
	{
		ft_putstr_fd("msh: unable to malloc\n", STDERR_FILENO);
		cenv.exit_status = 130;
		return ;
	}
	if (!getcwd(buf, 1000))
	{
		ft_putstr_fd("msh: the pwd size is bigger than 1000 char\n", STDERR_FILENO);
		cenv.exit_status = 130;
		return ;
	}
	ft_putstr_fd(buf, fd[1]);
	ft_putchar_fd('\n', fd[1]);
	cenv.exit_status = 0;
}