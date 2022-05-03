#include "builtins.h"
#include "minishell.h"

int		str_is_alphanum(char *str)
{
	while (*str)
	{
		if (!ft_isalnum(*str))
			return (0);
		str++;
	}
	return (1);
}

void	exec_builtin_unset(t_cmd node, int *fd)
{
	(void)fd;
	int i;

	i = 1;
	cenv.exit_status = 0;
	while (node.cmd_arg[i])
	{
		if (str_is_alphanum(node.cmd_arg[i]))
			del_param(node.cmd_arg[1]);
		else
		{
			ft_putstr_fd("msh: unset: ", STDERR_FILENO);
			ft_putstr_fd(node.cmd_arg[1], STDERR_FILENO);
			ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
			cenv.exit_status = 1;
		}
		i++;
	}
}