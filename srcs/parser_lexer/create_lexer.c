/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:58:43 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:58:44 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static t_lxr_type	lexer_type(char *str)
{
	if (*str == '(')
		return (scope_open);
	else if (*str == ')')
		return (scope_close);
	else if (*str == '\"')
		return (word);
	else if (*str == '\'')
		return (word);
	else if (!ft_strncmp(str, "&&", 2))
		return (sep_and);
	else if (!ft_strncmp(str, "||", 2))
		return (sep_or);
	else if (!ft_strncmp(str, "|", 1))
		return (sep_pipe);
	else if (!ft_strncmp(str, "<<", 2))
		return (heredoc);
	else if (!ft_strncmp(str, ">>", 2))
		return (append);
	else if (!ft_strncmp(str, ">", 1))
		return (open_out);
	else if (!ft_strncmp(str, "<", 1))
		return (read_in);
	else
		return (word);
}

static char	*fil_content_from_str(char *str)
{
	int		i;
	char	*res;

	i = 0;
	while (str[i] && str[i] != ' ' && lexer_type(str + i) == word)
	{
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
		}
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
		}
		if (str[i])
			i++;
	}
	res = (char *)malloc(sizeof(char) * (i + 1));
	ft_strlcpy(res, str, i + 1);
	return (res);
}

static void	scroll_str(char **str, t_lxr_type type)
{
	if (type == word)
	{
		while (**str && **str != ' ' && lexer_type(*str) == word)
		{
			if (**str == '\"')
			{
				(*str)++;
				while (**str && **str != '\"')
					(*str)++;
			}
			if (**str == '\'')
			{
				(*str)++;
				while (**str && **str != '\'')
					(*str)++;
			}
			if (**str)
				(*str)++;
		}
	}
	else if (type == heredoc || type == sep_and
		|| type == sep_or || type == append)
		(*str) += 2;
	else
		(*str)++;
}

t_list	*create_lexer(char *str)
{
	t_lxr	*lxr;
	t_list	*res;

	while (*str == ' ' || *str == '\r' || *str == '\n'
		|| *str == '\f' || *str == '\v' || *str == '\t')
		str++;
	if (!*str)
		return (NULL);
	lxr = (t_lxr *)malloc(sizeof(t_lxr));
	lxr->type = lexer_type(str);
	if (lxr->type == word)
		lxr->content = fil_content_from_str(str);
	else
		lxr->content = NULL;
	scroll_str(&str, lxr->type);
	res = ft_lstnew(lxr);
	ft_lstadd_back(&res, create_lexer(str));
	return (res);
}
