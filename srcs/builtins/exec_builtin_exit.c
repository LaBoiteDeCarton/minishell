#include "builtins.h"
#include "minishell.h"
#include <stdlib.h>

void	exec_builtin_exit(t_cmd node, int *fd)
{
	(void)node; //il faudrait free les noeuds....comment faire?

	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] > 0)
		close(fd[0]);
	//expand l'arg
	//ecrire uniquement en mode interactif
	msh_exit();
}