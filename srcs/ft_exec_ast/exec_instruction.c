#include "ast.h"

/*
	Exec instruction :
	Si il existe des redirection et si les redirections ont reussies et si la commande existe ...
	Si il n'existe pas de redirection et si la commande existe ...

	Valeur retour :
	- valeur retour de l'execution de la commande si execution effectué
	- 1 si echec dans l'ouverture d'un fichier
	- 0 sinon
*/
int	exec_instruction(t_instruction node)
{
	if (node.redirection && n_redirect(node.redirection) // valeurs retour ATTENTION
			&& node.cmd)
		return (exec_cmd(*node.cmd));
	else if (!node.redirection && node.cmd)
		return (exec_cmd(*node.cmd));
	return (0);
}