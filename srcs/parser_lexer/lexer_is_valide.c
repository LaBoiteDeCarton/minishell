/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_is_valide.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:59:09 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:59:10 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
	should_be :
		0 - whatever
		1 - a word
		2 - not a token
		not a scope etc..
*/

static int	what_is_next(t_lxr_type type)
{
	if (type == word)
		return (0);
	if (type == read_in || type == open_out || type == append | type == heredoc)
		return (1);
	if (type == sep_and || type == sep_or || type == sep_pipe)
		return (0);
	return (0);
}

static int	is_token(t_lxr_type type)
{
	return (type == sep_and || type == sep_or || type == sep_pipe);
}

static int	lexer_error(char *err)
{
	ft_putstr_fd("msh: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (0);
}

static char	*get_str_lxr(t_lxr_type type)
{
	if (type == scope_open)
		return ("(");
	if (type == scope_close)
		return (")");
	if (type == read_in)
		return ("<");
	if (type == open_out)
		return (">");
	if (type == append)
		return (">>");
	if (type == heredoc)
		return ("<<");
	if (type == sep_and)
		return ("&&");
	if (type == sep_or)
		return ("||");
	if (type == sep_pipe)
		return ("|");
	return ("newline");
}

static int	check_quote_parity(char *str)
{
	int	dq;
	int	sq;

	dq = 0;
	sq = 0;
	while (*str)
	{
		if (*str == '\"' && !sq)
			dq = (dq + 1) % 2;
		if (*str == '\'' && !dq)
			sq = (sq + 1) % 2;
		str++;
	}
	return (sq || dq);
}

int	lexer_is_valide(t_list	*lst)
{
	t_list	*ptr;	
	int		should_be;
	int		scope_count;

	ptr = lst;
	should_be = 0;
	scope_count = 0;
	while (ptr)
	{
		if (((t_lxr *)ptr->content)->type == scope_open)
			scope_count++;
		else if (((t_lxr *)ptr->content)->type == scope_close && !scope_count)
			return (lexer_error(")"));
		else if (((t_lxr *)ptr->content)->type == scope_close)
			scope_count--;
		else if (should_be == 1 && ((t_lxr *)ptr->content)->type != word)
			return (lexer_error(get_str_lxr(((t_lxr *)ptr->content)->type)));
		else if (should_be == 2 && is_token(((t_lxr *)ptr->content)->type))
			return (0);
		if (((t_lxr *)ptr->content)->type == word
			&& check_quote_parity(((t_lxr *)ptr->content)->content))
			return (lexer_error("\"' or `'"));
		should_be = what_is_next(((t_lxr *)ptr->content)->type);
		ptr = ptr->next;
	}
	if (scope_count != 0 || should_be)
		return (lexer_error("newline"));
	return (1);
}
