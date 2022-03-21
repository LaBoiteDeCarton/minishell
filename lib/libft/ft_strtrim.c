/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 12:17:48 by dmercadi          #+#    #+#             */
/*   Updated: 2021/03/25 22:42:49 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isinset(char c, char const *set)
{
	while (*set && set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static int	trimstart(char const *str, char const *set)
{
	int	start;

	start = 0;
	while (isinset(str[start], set) && str[start])
		start++;
	if (!str[start])
		return (0);
	return (start);
}

static int	trimend(char const *str, char const *set)
{
	int	end;

	end = ft_strlen(str);
	while (isinset(str[end - 1], set) && end >= 0)
		end--;
	if (end < 0)
		return (ft_strlen(str));
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		i;
	char	*trim;

	if (!s1)
		return (NULL);
	start = trimstart(s1, set);
	end = trimend(s1, set);
	if (start > end)
		return (NULL);
	trim = malloc(sizeof(char) * (end - start + 1));
	if (!trim)
		return (NULL);
	i = 0;
	while (start < end)
	{
		trim[i] = s1[start];
		start++;
		i++;
	}
	trim[i] = '\0';
	return (trim);
}
