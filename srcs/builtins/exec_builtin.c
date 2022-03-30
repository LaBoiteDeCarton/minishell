#include "builtins.h"

void	exec_builtin(t_cmd node, int *fd)
{
	t_builtin bi_type;

	bi_type = get_builtin(node.cmd_name);
	if (bi_type == bi_echo)
		exec_builtin_echo(node, fd);
	if (bi_type == bi_cd)
		exec_builtin_cd(node, fd);
}