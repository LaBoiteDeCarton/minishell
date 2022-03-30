#include "ast.h"
#include "minishell.h"

/*
	Exec instruction :
	Si il existe des redictions : effectue les redirections.
		Si les redirections echoue (valeur de retour == 0) alors on termine sans executer la commande
		la valeur du exit_status est de 1
	Si il existe une commande : on execute la commande.
	Sinon on met la valeur de exit_status à 0 (car l'instruction a reussie, même sans execution de commande)

	Valeur retour :
	- (None)
*/

#include <stdio.h>

void	exec_instruction(t_instruction node)
{
	int	red_success;
	
	red_success = 1;
	if (node.redirection)
		red_success = n_redirect(node.redirection, node.fd);
	if (!red_success)
	{
		cenv.exit_status = 1;
		return ;
	}
	if (node.cmd)
		exec_cmd(*node.cmd, node.fd);
	else
		cenv.exit_status = 0;
	return ;
}