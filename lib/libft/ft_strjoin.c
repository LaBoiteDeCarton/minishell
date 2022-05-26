/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:23:25 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 13:23:26 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	totsize;
	char	*new_s;

	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	if (!s1 && !s2)
		return (NULL);
	totsize = ft_strlen(s1) + ft_strlen(s2);
	new_s = malloc(sizeof(char) * (totsize + 1));
	if (!new_s)
		new_s = NULL;
	else
	{
		ft_strlcpy(new_s, s1, totsize + 1);
		ft_strlcat(new_s, s2, totsize + 1);
	}
	return (new_s);
}
