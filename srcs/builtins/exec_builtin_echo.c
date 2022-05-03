#include "builtins.h"
#include "minishell.h"

static int		arg_is_endloption(char *arg)
{
	if (!arg)
		return (0);
	if (!ft_strncmp(arg, "-n", 3))
		return (1);
	if (!ft_strncmp(arg, "-n", 2))
	{
		while (*(arg + 2))
		{
			if (*(arg + 2) != 'n')
				return (0);
			arg++;
		}
		return (1);
	}
	return (0);
}

void	exec_builtin_echo(t_cmd node, int *fd)
{
	int		endl;

	if (fd[1] == -1)
		fd[1] = dup(STDOUT_FILENO);
	if (fd[1] == -1)
		return (handle_errors("Echo"));
	(node.cmd_arg)++;
	endl = 1;
	while (arg_is_endloption(*(node.cmd_arg)))
	{
		endl = 0;
		(node.cmd_arg)++;
	}
	while (*(node.cmd_arg))
	{
		ft_putstr_fd(*(node.cmd_arg), fd[1]);
		if (*(node.cmd_arg + 1)) //&& ft_strlen(*(node.cmd_arg) faire avec argsize (changer la structure, si NULL ne pas metre d'espace, si /0 c'est different et donc on met un espace)
			ft_putchar_fd(' ', fd[1]);
		(node.cmd_arg)++;
	}
	if (endl)
		ft_putchar_fd('\n', fd[1]);
	cenv.exit_status = 0;
}