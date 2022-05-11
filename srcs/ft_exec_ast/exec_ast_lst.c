#include "ast.h"

void	exec_ast_lst(t_list *ast_lst)
{
	while (ast_lst)
	{
		exec_ast((t_ast *)ast_lst->content);
		ast_lst = ast_lst->next;
	}
}