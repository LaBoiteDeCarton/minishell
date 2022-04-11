#include "builtins.h"
#include "minishell.h"

void	exec_builtin_env(t_cmd node, int *fd)
{
	(void)node;
	char	**chartab;

	if (fd[1] <= 0)
		fd[1] = dup(STDOUT_FILENO);
	if (fd[1] == -1)
		return (handle_errors("Env: "));
	chartab = cenv.env;
	while (*chartab)
	{
		ft_putendl_fd(*chartab, fd[1]);
		chartab++;
	}
	cenv.exit_status = 0;
}