/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 14:54:15 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 14:54:16 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ast.h"

t_redirect_type	get_red(t_list *lst)
{
	if (((t_lxr *)lst->content)->type == read_in)
		return (red_in);
	if (((t_lxr *)lst->content)->type == open_out)
		return (red_out);
	if (((t_lxr *)lst->content)->type == append)
		return (red_append);
	if (((t_lxr *)lst->content)->type == heredoc)
		return (red_heredoc);
	return (red_error);
}

t_token	translate_token(t_list *lexer)
{
	t_lxr_type	lxr_type;

	lxr_type = get_type(lexer);
	if (lxr_type == sep_and)
		return (token_and);
	if (lxr_type == sep_or)
		return (token_or);
	if (lxr_type == sep_pipe)
		return (token_pipe);
	return (token_instruction);
}
