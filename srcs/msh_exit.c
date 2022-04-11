#include "minishell.h"

void	msh_exit()
{
	free_chartab(cenv.env);
	free_var(cenv.var);
	rl_clear_history();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(cenv.exit_status);
}