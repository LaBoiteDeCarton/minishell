/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:00 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:02 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

void	exec_ast(t_ast *ast)
{
	if (ast->token == token_execute)
		exec_ast_lst(ast->content);
	if (ast->token == token_instruction)
		exec_instruction((t_instruction *)ast->content->content);
	if (ast->token == token_and)
		exec_and(ast);
	if (ast->token == token_or)
		exec_or(ast);
	if (ast->token == token_pipe)
		exec_pipe(ast);
}
