#include "ast.h"

int	exec_and(t_and node)
{
	while (node.content)
	{
		if (!exec_ast(node.content))
			return (0);
		node.content++;
	}
	return (1);
}