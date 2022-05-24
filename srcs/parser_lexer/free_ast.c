/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:51:18 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:51:20 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

static void	free_redirection(void *content)
{
	t_redirect	*redirect;

	redirect = (t_redirect *)content;
	if (redirect->pathfile)
		free(redirect->pathfile);
	free(redirect);
}

static void	free_instruction(void *content)
{
	t_instruction	*instr;

	instr = (t_instruction *)content;
	ft_lstclear(&instr->redirection, &free_redirection);
	if (instr->cmd)
	{
		if (instr->cmd->cmd_arg)
			free_chartab(instr->cmd->cmd_arg);
		free(instr->cmd);
	}
	free(instr);
}

void	free_ast(void *content)
{
	t_ast	*ast;

	ast = (t_ast *)content;
	if (ast->token == token_instruction)
		ft_lstclear(&ast->content, &free_instruction);
	else
		ft_lstclear(&ast->content, &free_ast);
	free(ast);
}
