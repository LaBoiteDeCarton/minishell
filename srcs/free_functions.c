/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:31:21 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:31:23 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_chartab(char **chartab)
{
	int	i;

	i = 0;
	while (chartab[i])
		free(chartab[i++]);
	free(chartab);
}

void	clear_var(void *var)
{
	if (((t_var *)var)->name)
		free(((t_var *)var)->name);
	if (((t_var *)var)->value)
		free(((t_var *)var)->value);
	free((t_var *)var);
}

void	free_var(t_list *var)
{
	ft_lstclear(&var, &clear_var);
}
