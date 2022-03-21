/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 11:46:44 by dmercadi          #+#    #+#             */
/*   Updated: 2021/03/24 17:40:51 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_split(char **splited, int i)
{
	while (i--)
		free(splited[i]);
	free(splited);
	return (NULL);
}

static int	nbwords(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static int	ft_malloc_word(char const *s, char **word, char c)
{
	int	len;

	while (*s && *s == c)
		s++;
	len = 0;
	while (s[len] && s[len] != c)
		len++;
	*word = malloc(sizeof(char *) * (len + 1));
	if (!(*word))
		return (0);
	return (1);
}

static void	ft_copy_word(char const **s, char *word, char c)
{
	int	cpy;

	while (**s && **s == c)
		(*s)++;
	cpy = 0;
	while (**s && (**s) != c)
		word[cpy++] = *((*s)++);
	word[cpy] = 0;
}

char	**ft_split(char const *s, char c)
{
	char	**split_tab;
	int		i;
	int		size;

	if (!s)
		return (NULL);
	size = nbwords(s, c);
	split_tab = (char **)malloc(sizeof(char *) * (size + 1));
	if (!split_tab)
		return (NULL);
	i = -1;
	while (++i < size)
	{
		if (!ft_malloc_word(s, &(split_tab[i]), c))
			return (free_split(split_tab, i));
		ft_copy_word(&s, split_tab[i], c);
	}
	split_tab[size] = NULL;
	return (split_tab);
}
