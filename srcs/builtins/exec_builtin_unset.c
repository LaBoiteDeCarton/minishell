#include "builtins.h"
#include "minishell.h"

void	exec_builtin_unset(t_cmd node, int *fd)
{
	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] > 0)
		close(fd[0]);
	(void)node;
}