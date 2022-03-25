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

#include <stdio.h>

int	exec_instruction(t_instruction node)
{
	if (!node.redirection)
		printf("redirection est NULL\n");
	else
		printf("redirection is somehow not NULL\n");
	if (node.redirection)
		n_redirect(node.redirection);
	if (node.cmd)
		return (exec_cmd(*node.cmd));
	return (0);
}