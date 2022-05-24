/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_params.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:11:51 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 16:11:52 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

#include <stdio.h>
static void	print_param(t_var	*var)
{
	printf("VAR\nVAR NAME: %s\nVAR VALUE: %s\n", var->name, var->value);
}

void	print_params()
{
	t_list	*ptr;

	ptr = g_cenv.var;
	while (ptr)
	{
		print_param((t_var *)ptr->content);
		ptr = ptr->next;
	}
}
