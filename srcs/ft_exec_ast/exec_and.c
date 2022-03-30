#include "ast.h"
#include "minishell.h"

/*
	Exec AND :
	Execution iterative sur la liste de ast.
	Si execution reussie (i.e valeur de exit_status == 0) alors on execute l'element suivant
	Sinon l'execution est arreté et la valeur de retour est envoyé

	Valeur retour :
	- (None)
*/

void	exec_and(t_ast node)
{
	while (node.content)
	{
		exec_ast(*(t_ast *)node.content->content);
		if (cenv.exit_status)
			break ;
		node.content = node.content->next;
	}
}
