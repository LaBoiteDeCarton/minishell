/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:23:39 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 13:23:40 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!dst || !src)
		return (0);
	while (dstsize > 1 && src[i])
	{
		dst[i] = src[i];
		i++;
		dstsize--;
	}
	if (dstsize != 0)
		dst[i] = 0;
	while (src[i])
		i++;
	return (i);
}
