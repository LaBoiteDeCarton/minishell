#include "ast.h"

int	exec_or(t_or node)
{
	while (node.content)
	{
		if (exec_ast(*(t_ast *)node.content))
			return (0);
		node.content++;
	}
	return (1);
}