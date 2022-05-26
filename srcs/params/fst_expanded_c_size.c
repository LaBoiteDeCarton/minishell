/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fst_expanded_c_size.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 19:00:26 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 19:00:28 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	itoasize(int n)
{
	int	size;

	if (n < 0)
		size = 2;
	else
		size = 1;
	while (n / 10)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static int	expanded_squote_char_size(char **str)
{
	int	expanded_size;

	expanded_size = 1;
	(*str)++;
	while (**str && **str != '\'')
	{
		expanded_size++;
		(*str)++;
	}
	return (expanded_size);
}

static int	expanded_dquote_char_size(char **str)
{
	int	expanded_size;

	expanded_size = 1;
	(*str)++;
	while (**str && **str != '"')
	{
		if (!ft_strncmp(*str, "$?", 2))
		{
			expanded_size += itoasize(g_cenv.exit_status);
			(*str)++;
		}
		if (**str == '$')
		{
			expanded_size += ft_strlen(get_value(*str + 1));
			*str += var_name_size(*str + 1);
		}
		else
			expanded_size++;
		(*str)++;
	}
	return (expanded_size);
}

int	fst_expanded_c_size(char *str)
{
	int		expanded_size;

	expanded_size = 0;
	while (*str)
	{
		if (*str == '\"')
			expanded_size += expanded_dquote_char_size(&str);
		if (*str == '\'')
			expanded_size += expanded_squote_char_size(&str);
		if (!ft_strncmp(str, "$?", 2))
		{
			expanded_size += itoasize(g_cenv.exit_status);
			str++;
		}
		else if (*str == '$')
		{
			expanded_size += ft_strlen(get_value(str + 1));
			str += var_name_size(str + 1);
		}
		else
			expanded_size++;
		str++;
	}
	return (expanded_size + 1);
}
