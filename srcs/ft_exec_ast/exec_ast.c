#include "ast.h"

int	exec_ast(t_ast ast)
{
	if (ast.token == token_instruction)
		return (exec_instruction(*(t_instruction *)ast.content));
	if (ast.token == token_and)
		return (exec_and(ast));
	if (ast.token == token_or)
		return (exec_or(ast));
	if (ast.token == token_pipe)
		return (exec_pipe(ast));
	return (0);
}