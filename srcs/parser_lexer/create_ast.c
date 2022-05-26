/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:58:37 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:58:38 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"

static void	scroll_lexer_to_next_token(t_list	**lexer)
{
	int	scope_count;

	scope_count = 0;
	if (get_type(*lexer) == scope_open)
		scope_count = 1;
	*lexer = (*lexer)->next;
	while (*lexer)
	{
		if (get_type(*lexer) == scope_open)
			scope_count++;
		if (get_type(*lexer) == scope_close)
			scope_count--;
		if (is_token(*lexer) && get_type(*lexer) != sep_pipe && !scope_count)
			break ;
		*lexer = (*lexer)->next;
		if (scope_count < 0)
			*lexer = NULL;
	}
}

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

t_ast	*next_ast(t_list **lexer, int piping)
{
	t_ast	*ast;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
		return (handle_ast_error(ast, NULL, "parsing"));
	ast->token = translate_token(*lexer);
	if (ast->token == token_instruction && piping && is_pipe(*lexer))
	{
		ast->token = token_execute;
		ast->content = ft_lstnew(from_lexer_to_pipe(*lexer));
	}
	else if (get_type(*lexer) == scope_open)
	{
		ast->token = token_execute;
		ast->content = create_ast_lst((*lexer)->next);
	}
	else if (ast->token == token_instruction)
		ast->content = ft_lstnew(from_lexer_to_instruction(*lexer));
	else
		ast->content = ft_lstnew(next_ast(&((*lexer)->next), 1));
	if (!ast->content)
		return (handle_ast_error(ast, NULL, "parsing"));
	return (ast);
}

t_list	*create_ast_lst(t_list *lexer)
{
	t_list	*ast_lst;

	ast_lst = NULL;
	while (lexer)
	{
		ft_lstadd_back(&ast_lst, ft_lstnew(next_ast(&lexer, 1)));
		scroll_lexer_to_next_token(&lexer);
	}
	return (ast_lst);
}
