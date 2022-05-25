/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:32:18 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:32:19 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"

/*	ft_system("ma commande") équivaut à system("ma commande")
	La commande est traduite en lexer (une liste de token/content)
	Si le lexer n'est pas valide
		la valeur de exit_status est 258 (il n'y a aucune execution)
	Sinon
		le lexer est traduit en liste d'Arbre de la syntaxe abstraite (AST)
		on lance l'execution de la liset d'AST
*/

/*
	To visualize the lexer and the AST, use this functions (need to be décommented)
	printf_lexer(lexer);
	printASTLIST(ast);
*/

void	ft_system(char *command)
{
	t_list	*lexer;

	lexer = create_lexer(command);
	if (!lexer_is_valide(lexer))
	{
		g_cenv.exit_status = 258;
		ft_lstclear(&lexer, &free_lexer);
		return ;
	}
	g_cenv.ast = create_ast_lst(lexer);
	ft_lstclear(&lexer, &free);
	if (g_cenv.ast)
		exec_ast_lst(g_cenv.ast);
	ft_lstclear(&g_cenv.ast, &free_ast);
}
