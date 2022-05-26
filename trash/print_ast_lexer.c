/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_lexer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:59:18 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:59:19 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include <stdio.h>

//TO DELETE

void	printf_lexer(t_list	*lst)
{
	while (lst)
	{
		if (((t_lxr *)lst->content)->type == scope_open)
			printf("scope_open\n");
		else if (((t_lxr *)lst->content)->type == scope_close)
			printf("scope_close\n");
		else if (((t_lxr *)lst->content)->type == read_in)
			printf("read_in\n");
		else if (((t_lxr *)lst->content)->type == open_out)
			printf("open_out\n");
		else if (((t_lxr *)lst->content)->type == append)
			printf("append\n");
		else if (((t_lxr *)lst->content)->type == sep_and)
			printf("sep_and\n");
		else if (((t_lxr *)lst->content)->type == sep_or)
			printf("sep_or\n");
		else if (((t_lxr *)lst->content)->type == sep_pipe)
			printf("sep_pipe\n");
		else if (((t_lxr *)lst->content)->type == heredoc)
			printf("heredoc\n");
		else if(((t_lxr *)lst->content)->type == word)
		{
			printf("word :");
			printf("%s\n", ((t_lxr *)lst->content)->content);
		}
		lst = lst->next;
	}
}


void	write_nchar(char c, int n)
{
	while (n-- > 0)
		write(1, &c, 1);
}


void	print_token(t_token token)
{
	if (token == token_and)
		printf("AND\n");
	if (token == token_or)
		printf("OR\n");
	if (token == token_pipe)
		printf("PIPE\n");
	if (token == token_instruction)
		printf("INSTR\n");
	if (token == token_execute)
		printf("EXEC\n");
}

void	print_instr(t_instruction instr, int prof)
{
	int i;

	if (!instr.cmd)
	{
		write_nchar('|', prof);
		printf("CMD NULL\n");
	}
	if (instr.cmd)
	{
		write_nchar('|', prof);
		printf("CMD : %s\n", instr.cmd->cmd_name);
		i = 0;
		while (instr.cmd->cmd_arg[i])
		{
			write_nchar('|', prof);
			printf("%d-ieme arg : %s\n", i, instr.cmd->cmd_arg[i]);
			i++;
		}
	}
	if (!instr.redirection)
	{
		write_nchar('|', prof);
		printf("NO REDIRECTIONS\n");
	}
	if (instr.redirection)
	{
		write_nchar('|', prof);
		if (((t_redirect *)instr.redirection->content)->pathfile)
			printf("RED PATHFILE : %s\n", ((t_redirect *)instr.redirection->content)->pathfile);
		else
			printf("IS NULL");
	}
}

void	printAST(t_ast *ast, int prof)
{
	t_list *ptr;
	write_nchar('|', prof);
	print_token(ast->token);
	if (ast->token != token_instruction)
	{
		ptr = ast->content;
		while (ptr)
		{
			printAST((t_ast *)ptr->content, prof + 1);
			ptr = ptr->next;
		}
	}
	else
		print_instr(*(t_instruction *)(ast->content->content), prof + 1);
}

void printASTLIST(t_list *ast)
{
	while (ast)
	{
		printAST((t_ast *)ast->content, 0);
		ast = ast->next;
	}
}
