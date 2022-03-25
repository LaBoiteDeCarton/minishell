#include "ast.h"

int	exec_ast(t_ast ast)
{
	if (ast.token == token_instruction)
		return (exec_instruction(*(t_instruction *)ast.content));
	if (ast.token == token_and)
		return (exec_and(*(t_and *)ast.content));
	if (ast.token == token_or)
		return (exec_or(*(t_or *)ast.content));
	if (ast.token == token_pipe)
		return (exec_pipe(*(t_pipe *)ast.content));
	return (0);
}