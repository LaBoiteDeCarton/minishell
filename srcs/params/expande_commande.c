#include "minishell.h"
#include "ast.h"



void	expande_commande(t_cmd *node)
{
	char	*name_save;
	char	*arg_save;
	int		i;

	name_save = node->cmd_name;
	node->cmd_name = expande_char(node->cmd_name);
	free(name_save);
	node->cmd_arg[0] = node->cmd_name;
	i = 1;
	while (node->cmd_arg[i])
	{
		//ici faire un truc compliquer
		arg_save = node->cmd_arg[i];
		node->cmd_arg[i] = expande_char(node->cmd_arg[i]);
		free(arg_save);
		i++;
	}
}