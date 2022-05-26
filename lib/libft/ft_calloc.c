/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:11:41 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 13:11:42 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*buff;

	buff = malloc(count * size);
	if (!buff)
	{
		errno = ENOMEM;
		return (NULL);
	}
	ft_bzero(buff, count * size);
	return (buff);
}
