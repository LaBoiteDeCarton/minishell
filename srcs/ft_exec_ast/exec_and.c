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

void	exec_and(t_ast *node)
{
	t_list	*ptr;

	ptr = node->content;
	while (ptr)
	{
		exec_ast((t_ast *)ptr->content);
		if (cenv.exit_status)
			break ;
		ptr = ptr->next;
	}
}
