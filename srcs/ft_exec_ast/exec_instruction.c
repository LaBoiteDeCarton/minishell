/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_instruction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:24 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:27 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

/*
	Exec instruction :
	Si TOUS les arguments sont des attributions de variables, on effectue 
	les attributions
		Sinon on ignore les attribution
	Si il existe des redictions : effectue les redirections.
		Si les redirections echoue (valeur de retour == 0) alors on termine 
		sans executer la commande
		la valeur du exit_status est de 1
	Si il existe une commande : on execute la commande.
	Sinon on met la valeur de exit_status à 0 (car l'instruction a reussie, 
	même sans execution de commande)

	Valeur retour :
	- (None)
*/

static int	n_redirect(t_list *redirections, int *fd)
{
	t_list	*ptr;

	ptr = redirections;
	while (ptr)
	{
		if (!redirect(*(t_redirect *)ptr->content, fd))
			return (0);
		ptr = ptr->next;
	}
	return (1);
}

static void	skip_var_attribution(t_cmd *cmd)
{
	int		i;
	char	**ptr;

	if (!cmd)
		return ;
	i = 0;
	while (char_is_var_attribution(cmd->cmd_arg[i]))
	{
		free(cmd->cmd_arg[i]);
		cmd->cmd_arg[i] = NULL;
		i++;
	}
	if (i == 0)
		return ;
	ptr = cmd->cmd_arg + i;
	while (*ptr)
	{
		*(ptr - i) = *ptr;
		*ptr = NULL;
		ptr++;
	}
	cmd->cmd_name = cmd->cmd_arg[0];
}

void	exec_instruction(t_instruction *node)
{
	int	red_success;
	int	do_not_exec_cmd;

	do_not_exec_cmd = cmd_is_var_attribution(node->cmd);
	if (do_not_exec_cmd)
		add_params(node->cmd->cmd_arg);
	else
		skip_var_attribution(node->cmd);
	red_success = 1;
	if (node->redirection)
		red_success = n_redirect(node->redirection, node->fd);
	if (!red_success)
	{
		g_cenv.exit_status = 1;
		return ;
	}
	if (!do_not_exec_cmd && node->cmd)
		exec_cmd(node->cmd, node->fd);
	else
		g_cenv.exit_status = 0;
	if (node->fd[0] != -1 && close(node->fd[0]) == -1)
		handle_errors("Command");
	if (node->fd[1] != -1 && close(node->fd[1]) == -1)
		handle_errors("Command");
	return ;
}
