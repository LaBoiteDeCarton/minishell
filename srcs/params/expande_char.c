/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expande_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:11:33 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 16:11:34 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	expande_variable(char *dst, int *start, char **str)
// {
// 	char	*itoachar;

// 	(*str)++;
// 	if (**str == '?')
// 	{
// 		itoachar = get_value(*str);
// 		ft_memcpy(dst + *start, itoachar, ft_strlen(itoachar));
// 		*start += ft_strlen(itoachar);
// 		free(itoachar);
// 		(*str)++;
// 	}
// 	else
// 	{
// 		ft_memcpy(dst + *start, get_value(*str), ft_strlen(get_value(*str)));
// 		*start += ft_strlen(get_value(*str));
// 		*str += var_name_size(*str);
// 	}
// }

// static void	expande_dquote_char(char *dst, int *start, char **str)
// {
// 	(*str)++;
// 	while (**str && **str != '"')
// 	{
// 		if (**str == '$')
// 			expande_variable(dst, start, str);
// 		else
// 		{
// 			dst[*start] = **str;
// 			(*start)++;
// 			(*str)++;
// 		}
// 	}
// 	if (**str)
// 		(*str)++;
// }

// static void	expande_squote_char(char *dst, int *start, char **str)
// {
// 	(*str)++;
// 	while (**str && **str != '\'')
// 	{
// 		dst[*start] = **str;
// 		(*start)++;
// 		(*str)++;
// 	}
// 	if (**str)
// 		(*str)++;
// }

void	ft_copy_without_quotes(char *dst, char *src)
{
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	dquote = 0;
	squote = 0;
	while (*src)
	{
		if (*src == '\"' && !squote)
			dquote = (dquote + 1) % 2;
		else if (*src == '\'' && !dquote)
			squote = (squote + 1) % 2;
		else
		{
			dst[i] = *src;
			i++;
		}
		src++;
	}
	dst[i] = '\0';
}

char	*expande_char(char *str)
{
	char	*fst_expande;
	char	*expande;

	fst_expande = first_expand(str);
	expande = (char *)malloc(sizeof(char) * ft_strlen(fst_expande) + 1);
	if (!expande)
		handle_errors("expande failed");
	ft_copy_without_quotes(expande, fst_expande);
	free(fst_expande);
	return (expande);
}
