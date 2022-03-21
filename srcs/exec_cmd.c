#include "ast.h"

int	exec_cmd(t_cmd node)
{
	int	pid_id;
	int	status;

	//check si builtin
	pid_id = fork();
	status = 1;
	if (pid_id == -1)
	{
		write(2, "Unable to fork\n", 15);
		return (-1);
	}
	else if (pid_id == 0)
		execve(node.cmd_name, node.cmd_arg, cenv);
	else
	{
		waitpid(pid_id, &status, 0);
	}
	return (status);
}