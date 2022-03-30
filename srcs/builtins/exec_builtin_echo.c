#include "builtins.h"

void	exec_builtin_echo(t_cmd node, int *fd)
{
	int	endl;

	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] <= 0)
		fd[1] = dup(STDOUT_FILENO);
	(node.cmd_arg)++;
	endl = 1;
	if (!ft_strncmp(*(node.cmd_arg), "-n", 3))
	{
		endl = 0;
		(node.cmd_arg)++;
	}
	while (*(node.cmd_arg))
	{
		ft_putstr_fd(*(node.cmd_arg), fd[1]);
		(node.cmd_arg)++;
		if (*(node.cmd_arg))
			ft_putchar_fd(' ', fd[1]);
	}
	if (endl)
		ft_putchar_fd('\n', fd[1]);
	close(fd[1]);
}