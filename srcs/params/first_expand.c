/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 18:45:37 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 18:45:38 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expande_variable(char *dst, int *start, char **str)
{
	char	*itoachar;

	if (!ft_strncmp(*str, "$?", 2))
	{
		(*str)++;
		itoachar = get_value(*str);
		ft_memcpy(dst + *start, itoachar, ft_strlen(itoachar));
		*start += ft_strlen(itoachar) - 1;
		free(itoachar);
	}
	else if (**str == '$')
	{
		(*str)++;
		ft_memcpy(dst + *start, get_value(*str), ft_strlen(get_value(*str)));
		*start += ft_strlen(get_value(*str)) - 1;
		*str += var_name_size(*str) - 1;
	}
}

static void	expande_dquote_char(char *dst, int *start, char **str)
{
	char	*itoachar;

	while (**str && **str != '"')
	{
		if (**str == '$')
			expande_variable(dst, start, str);
		else
			dst[*start] = **str;
		(*start)++;
		(*str)++;
	}
}

static void	expande_squote_char(char *dst, int *start, char **str)
{
	dst[*start] = **str;
	(*start)++;
	(*str)++;
	while (**str && **str != '\'')
	{
		dst[*start] = **str;
		(*start)++;
		(*str)++;
	}
}

#include <stdio.h>
char	*first_expand(char *str)
{
	char	*expanded_char;
	int		i;

	expanded_char = (char *)malloc(sizeof(char) * fst_expanded_c_size(str));
	if (!expanded_char)
		return (handle_expande_errors("expande failed: "));
	i = 0;
	while (*str)
	{
		if (*str == '\"')
		{
			expanded_char[i++] = *str;
			str++;
			expande_dquote_char(expanded_char, &i, &str);
		}
		else if (*str == '\'')
			expande_squote_char(expanded_char, &i, &str);
		if (*str == '$')
			expande_variable(expanded_char, &i, &str);
		else
			expanded_char[i] = *str;
		str++;
	}
	expanded_char[i] = '\0';
	printf("expanded char = %s\n", expanded_char);
	return (expanded_char);
}
