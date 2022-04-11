#include "minishell.h"

void	msh_exit()
{
	free_chartab(cenv.env);
	free_var(cenv.var);
	//free ast si existe
	rl_clear_history();
	ft_putstr_fd("exit\n", STDERR_FILENO);
	exit(cenv.exit_status);
}