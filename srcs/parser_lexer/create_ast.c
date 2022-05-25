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
#include "minishell.h"
#include "lexer.h"
#include "libft.h"
#include <stdio.h>

static t_lxr_type	get_type(t_list *lexer)
{
	if (!lexer)
		return (stop);
	return (((t_lxr *)lexer->content)->type);
}

static int	is_token(t_list *lst)
{
	t_lxr_type	lst_type;

	lst_type = get_type(lst);
	return (lst_type == sep_and
		|| lst_type == sep_or
		|| lst_type == sep_pipe
		|| lst_type == stop);
}

static int	is_redirection(t_list *lst)
{
	t_lxr_type	lst_type;

	lst_type = get_type(lst);
	return (lst_type == read_in
		|| lst_type == open_out
		|| lst_type == append
		|| lst_type == heredoc);
}

static int	is_text(t_list *lst)
{
	return (get_type(lst) == word);
}

static t_token	translate_token(t_list *lexer)
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

static int	get_arg_size(t_list *lst)
{
	int	nb_of_word;

	nb_of_word = 0;
	while (lst && !is_token(lst))
	{
		if (is_text(lst))
			nb_of_word++;
		if (is_redirection(lst))
			lst = lst->next;
		lst = lst->next;
	}
	return (nb_of_word);
}

static t_redirect_type	get_red(t_list *lst)
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

static t_list	*add_redirection(t_list *lst)
{
	t_list		*lst_of_redirect;
	t_redirect	*redirection;

	lst_of_redirect = NULL;
	while (lst && !is_token(lst))
	{
		if (is_redirection(lst))
		{
			redirection = (t_redirect *)malloc(sizeof(t_redirect));
			if (!redirection)
				break ;
			redirection->red_type = get_red(lst);
			lst = lst->next;
			redirection->pathfile = ((t_lxr *)lst->content)->content;
			if (redirection->red_type == red_heredoc)
				redirection->fd = get_heredoc(redirection->pathfile);
			ft_lstadd_back(&lst_of_redirect, ft_lstnew(redirection));
		}
		lst = lst->next;
	}
	return (lst_of_redirect);
}

static t_cmd	*add_cmd(t_list *lst)
{
	t_cmd	*cmd;
	int		argsize;
	int		i;

	argsize = get_arg_size(lst);
	if (!argsize)
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_name = NULL;
	cmd->cmd_arg = (char **)malloc(sizeof(char *) * (argsize + 1));
	if (!cmd->cmd_arg)
	{
		free(cmd);
		return (NULL);
	}
	cmd->cmd_arg[argsize] = NULL;
	i = 0;
	while (lst && !is_token(lst))
	{
		if (is_text(lst) && !cmd->cmd_name)
			cmd->cmd_name = ((t_lxr *)lst->content)->content;
		if (is_text(lst))
			cmd->cmd_arg[i++] = ((t_lxr *)lst->content)->content;
		if (is_redirection(lst))
			lst = lst->next;
		lst = lst->next;
	}
	return (cmd);
}

static t_instruction	*from_lexer_to_instruction(t_list *lexer)
{
	t_instruction	*instruction;

	if (!lexer)
		return (NULL);
	instruction = (t_instruction *)malloc(sizeof(t_instruction));
	if (!instruction)
	{
		handle_errors("parsing");
		return (NULL);
	}
	instruction->fd[0] = -1;
	instruction->fd[1] = -1;
	instruction->redirection = add_redirection(lexer);
	instruction->cmd = add_cmd(lexer);
	return (instruction);
}

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

static t_ast	*from_lexer_to_pipe(t_list *lexer)
{
	t_ast	*pipe;

	if (!lexer)
		return (NULL);
	pipe = (t_ast *)malloc(sizeof(t_ast));
	if (!pipe)
	{
		handle_errors("parsing");
		return (NULL);
	}
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

t_ast	*next_ast(t_list **lexer, int piping)
{
	t_ast	*ast;

	ast = (t_ast *)malloc(sizeof(t_ast));
	if (!ast)
	{
		handle_errors("parsing");
		return (NULL);
	}
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
	{
		handle_errors("parsing");
		free_ast(ast);
		return (NULL);
	}
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
