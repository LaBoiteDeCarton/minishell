/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_or.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:33 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:34 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
/*
	Exec OR :
	Execution iterative sur la liste de ast.
	Si execution echoue (i.e valeur de exit_status > 0) alors on execute 
		l'element suivant
	Sinon l'execution est arreté et exit_status est inchangé

	Valeur retour :
	- (None)
*/

void	exec_or(t_ast *node)
{
	t_list	*ptr;

	ptr = node->content;
	while (ptr && g_cenv.exit_status)
	{
		exec_ast((t_ast *)ptr->content);
		ptr = ptr->next;
	}
}
