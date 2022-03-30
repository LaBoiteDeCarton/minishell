#include "ast.h"
#include "minishell.h"
/*
	Exec OR :
	Execution iterative sur la liste de ast.
	Si execution echoue (i.e valeur de exit_status > 0) alors on execute l'element suivant
	Sinon l'execution est arreté et exit_status est inchangé

	Valeur retour :
	- (None)
*/

void	exec_or(t_ast node)
{
	while (node.content)
	{
		exec_ast(*(t_ast *)node.content->content);
		if (!cenv.exit_status)
			break ;
		node.content = node.content->next;
	}
}