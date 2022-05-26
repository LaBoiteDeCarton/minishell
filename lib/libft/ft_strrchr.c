/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:24:05 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 13:24:06 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*lastocc;

	lastocc = 0;
	while (*s)
	{
		if (*s == (char)c)
			lastocc = (char *)s;
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (lastocc);
}
