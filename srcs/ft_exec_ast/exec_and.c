#include "ast.h"

int	exec_and(t_ast node)
{
	while (node.content)
	{
		if (!exec_ast(*(t_ast *)node.content))
			return (0);
		node.content = node.content->next;
	}
	return (1);
}