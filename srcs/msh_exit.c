#include "minishell.h"

void	msh_exit()
{
	free_chartab(cenv.env);
	free_var(cenv.var);
	//free ast si existe
	rl_clear_history();
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	system("leaks minishell");
	exit(cenv.exit_status);
}