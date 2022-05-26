/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:23:31 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 13:23:32 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	while (dst[i] && dstsize > 0)
	{
		i++;
		dstsize--;
	}
	while (*src && dstsize > 1)
	{
		dst[i++] = *src++;
		dstsize--;
	}
	if (dstsize >= 1)
		dst[i] = '\0';
	while (*src++)
		i++;
	return (i);
}
