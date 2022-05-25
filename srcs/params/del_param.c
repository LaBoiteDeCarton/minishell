/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_param.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:44:19 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/25 16:44:20 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*del_from_var(char *name, t_list *var)
{
	t_list	*reste;

	if (!var)
		return (NULL);
	if (!ft_strncmp(name, ((t_var *)var->content)->name, ft_strlen(name) + 1))
	{
		reste = var->next;
		ft_lstdelone(var, &clear_var);
		return (del_from_var(name, reste));
	}
	var->next = del_from_var(name, var->next);
	return (var);
}

void	del_param(char *name)
{
	del_from_env(name);
	g_cenv.var = del_from_var(name, g_cenv.var);
}
