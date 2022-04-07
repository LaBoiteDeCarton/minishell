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
//The var attribution est de la forme TEXTE='word' il faut faire l'expansion de word avant de valider l'affectation
// tout attaché, sans expace autour de '='

static void	skip_var_attribution(t_cmd *cmd)
{
	while (char_is_var_attribution(*(cmd->cmd_arg)))
		cmd->cmd_arg++;
	cmd->cmd_name = cmd->cmd_arg[0];
}

void	exec_instruction(t_instruction node)
{
	int	red_success;
	int	do_not_exec_cmd;

	//if cmd is an VAR attribution && no other arguments, add to local params
	//if cmd is an VAR attribution && ther is other args, on doit ignorer la VAR attribution, et exeuter la suite
	//do the expansion of red char (directly in n_redirection)
	//if cmd wasnt a VAR attribution, do the expansion and execute
	do_not_exec_cmd = cmd_is_var_attribution(*(node.cmd));
	if (do_not_exec_cmd)
		add_params(node.cmd->cmd_arg);
	else
		skip_var_attribution(node.cmd);
	red_success = 1;
	if (node.redirection)
		red_success = n_redirect(node.redirection, node.fd);
	if (!red_success)
	{
		cenv.exit_status = 1;
		return ;
	}
	if (node.cmd && !do_not_exec_cmd)
		exec_cmd(*node.cmd, node.fd);
	else
		cenv.exit_status = 0;
	return ;
}