/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_var_attribution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:11:47 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 16:11:48 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

int	char_is_var_attribution(char *str)
{
	if (!str || !*str || !ft_isalpha(*str))
		return (0);
	while (ft_isalnum(*str))
		str++;
	if (!*str || *str != '=')
		return (0);
	return (1);
}

int	cmd_is_var_attribution(t_cmd *node)
{
	char	**chartab;

	if (!node)
		return (0);
	chartab = node->cmd_arg;
	while (*chartab)
	{
		if (!char_is_var_attribution(*(chartab)))
			return (0);
		chartab++;
	}
	return (1);
}
