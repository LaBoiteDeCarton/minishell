/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast_instruction.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:43:46 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/26 15:43:47 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"

static int	get_arg_size(t_list *lst)
{
	int	nb_of_word;

	nb_of_word = 0;
	while (lst && !is_token(lst))
	{
		if (get_type(lst) == word)
			nb_of_word++;
		if (is_redirection(lst))
			lst = lst->next;
		lst = lst->next;
	}
	return (nb_of_word);
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
				return (handle_ast_error(NULL, NULL, "parsing"));
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
	int		i;

	if (!get_arg_size(lst))
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_arg = (char **)malloc(sizeof(char *) * (get_arg_size(lst) + 1));
	if (!cmd->cmd_arg)
		return (handle_ast_error(NULL, cmd, "parsing"));
	i = 0;
	while (lst && !is_token(lst))
	{
		if (get_type(lst) == word)
			cmd->cmd_arg[i++] = ((t_lxr *)lst->content)->content;
		if (is_redirection(lst))
			lst = lst->next;
		lst = lst->next;
	}
	cmd->cmd_arg[i] = NULL;
	cmd->cmd_name = cmd->cmd_arg[0];
	return (cmd);
}

t_instruction	*from_lexer_to_instruction(t_list *lexer)
{
	t_instruction	*instruction;

	if (!lexer)
		return (NULL);
	instruction = (t_instruction *)malloc(sizeof(t_instruction));
	if (!instruction)
		return (handle_ast_error(NULL, NULL, "parsing"));
	instruction->fd[0] = -1;
	instruction->fd[1] = -1;
	instruction->redirection = add_redirection(lexer);
	instruction->cmd = add_cmd(lexer);
	return (instruction);
}
