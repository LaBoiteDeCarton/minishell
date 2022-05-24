/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:58:59 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:59:00 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_lexer(void *content)
{
	if (((t_lxr *)content)->content)
		free(((t_lxr *)content)->content);
	free((t_lxr *)content);
}
