/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:44:01 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 15:44:02 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"

static int	is_pipe(t_list *lexer)
{
	int	scope_count;

	scope_count = 0;
	while (lexer)
	{
		if (get_type(lexer) == scope_open)
			scope_count++;
		if (get_type(lexer) == scope_close)
			scope_count--;
		if (is_token(lexer) && !scope_count)
			break ;
		lexer = lexer->next;
	}
	if (get_type(lexer) == sep_pipe)
		return (1);
	return (0);
}

static void	scroll_lexer_to_next_pipe(t_list **lexer)
{
	while (*lexer)
	{
		if (is_token(*lexer) && get_type(*lexer) == sep_pipe)
		{
			*lexer = (*lexer)->next;
			break ;
		}
		(*lexer) = (*lexer)->next;
	}
}

t_ast	*from_lexer_to_pipe(t_list *lexer)
{
	t_ast	*pipe;

	if (!lexer)
		return (NULL);
	pipe = (t_ast *)malloc(sizeof(t_ast));
	if (!pipe)
		return (handle_ast_error(pipe, NULL, "parsing"));
	pipe->token = token_pipe;
	pipe->content = NULL;
	ft_lstadd_back(&(pipe->content), ft_lstnew(next_ast(&lexer, 0)));
	while (lexer && is_pipe(lexer))
	{
		scroll_lexer_to_next_pipe(&lexer);
		ft_lstadd_back(&(pipe->content), ft_lstnew(next_ast(&lexer, 0)));
	}
	return (pipe);
}
