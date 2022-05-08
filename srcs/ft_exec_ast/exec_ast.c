#include "ast.h"

void	exec_ast(t_ast *ast)
{
	if (ast->token == token_instruction)
		exec_instruction((t_instruction *)ast->content->content);
	if (ast->token == token_and)
		exec_and(ast);
	if (ast->token == token_or)
		exec_or(ast);
	if (ast->token == token_pipe)
		exec_pipe(ast);
}