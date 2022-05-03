#include "minishell.h"
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"

/*	ft_system("ma commande") équivaut à system("ma commande")
	La commande est traduite en lexer (une liste de token/content)
	Si le lexer n'est pas valide
		la valeur de exit_status est 258 (il n'y a aucune execution)
	Sinon
		le lexer est traduit en Arbre de la syntaxe abstraite (AST)
		on lance l'execution
*/

void	ft_system(char *command)
{
	t_list	*lexer;
	t_ast	*ast;
	// system(command);

	lexer = create_lexer(command);
	//printf_lexer(lexer);
	if (!lexer_is_valide(lexer))
	{
		cenv.exit_status = 258;
		ft_lstclear(&lexer, &free);
		return ;
	}
	ast = from_lexer_to_ast(lexer);
	//printAST(ast, 0);
	if (ast)
		exec_ast(*ast);
	ft_lstclear(&lexer, &free); //lexer ne free pas les char* mais le AST doit le faire lui
	//free ast
}