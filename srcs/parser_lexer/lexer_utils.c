/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 14:45:25 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 14:45:26 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lxr_type	get_type(t_list *lexer)
{
	if (!lexer)
		return (stop);
	return (((t_lxr *)lexer->content)->type);
}

int	is_token(t_list *lst)
{
	t_lxr_type	lst_type;

	lst_type = get_type(lst);
	return (lst_type == sep_and
		|| lst_type == sep_or
		|| lst_type == sep_pipe
		|| lst_type == stop);
}

int	is_redirection(t_list *lst)
{
	t_lxr_type	lst_type;

	lst_type = get_type(lst);
	return (lst_type == read_in
		|| lst_type == open_out
		|| lst_type == append
		|| lst_type == heredoc);
}
