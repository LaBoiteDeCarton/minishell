#include "builtins.h"
#include "minishell.h"

void	exec_builtin_env(t_cmd node, int *fd)
{
	(void)node;
	char	**chartab;

	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] <= 0)
		fd[1] = dup(STDOUT_FILENO); //attention catch l'erreur ici
	chartab = cenv.env;
	while (*chartab)
	{
		ft_putendl_fd(*chartab, fd[1]);
		chartab++;
	}
	close(fd[1]);
	cenv.exit_status = 0;
}