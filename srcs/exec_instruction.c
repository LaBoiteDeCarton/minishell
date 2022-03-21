#include "ast.h"

void	exec_instruction(t_instruction node)
{
	(void)node;
	if (node.redirect && n_redirect(node.redirect))
		exec_cmd(node.cmd);
}