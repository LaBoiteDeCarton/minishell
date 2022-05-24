/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:10 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:13 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

void	exec_ast_lst(t_list *ast_lst)
{
	while (ast_lst)
	{
		exec_ast((t_ast *)ast_lst->content);
		ast_lst = ast_lst->next;
	}
}
