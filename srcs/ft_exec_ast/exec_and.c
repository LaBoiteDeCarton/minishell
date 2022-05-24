/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:10:45 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:10:47 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

/*
	Exec AND :
	Execution iterative sur la liste de ast.
	Si execution reussie (i.e valeur de exit_status == 0) alors on execute 
	l'element suivant
	Sinon l'execution est arreté et la valeur de retour est envoyé

	Valeur retour :
	- (None)
*/

void	exec_and(t_ast *node)
{
	t_list	*ptr;

	ptr = node->content;
	while (ptr && !g_cenv.exit_status)
	{
		exec_ast((t_ast *)ptr->content);
		ptr = ptr->next;
	}
}
