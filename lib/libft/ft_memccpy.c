/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:13:30 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 13:13:31 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	while (n-- > 0)
	{
		*((unsigned char *)dst) = *((unsigned const char *)src);
		if (*((unsigned char *)src) == (unsigned char)c)
			return (dst + 1);
		dst++;
		src++;
	}
	return (NULL);
}
