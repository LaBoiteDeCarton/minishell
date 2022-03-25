#include "minishell.h"
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"

void	ft_system(char *command)
{
	t_list	*lexer;
	t_ast *ast;
	// system(command);

	lexer = create_lexer(command);
	if (!lexer_is_valide(lexer))
		return ; // code erreur 258
	ast = from_lexer_to_ast(lexer);
	//exec_ast(*ast);
	printAST(ast, 0);
	ft_lstclear(&lexer, &free_content);
	//free ast
}