/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:34:35 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:34:37 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	*handle_ast_error(t_ast *ast, t_cmd *cmd, char *str)
{
	if (ast)
		free_ast(ast);
	if (cmd)
		free(cmd);
	handle_errors(str);
	return (NULL);
}

int	lexer_error(char *err)
{
	ft_putstr_fd("msh: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (0);
}

t_list	*handle_wildcard_error(char *dir)
{
	handle_errors(dir);
	if (dir)
		free(dir);
	return (NULL);
}

void	handle_errors(char *msg_error)
{
	ft_putstr_fd("msh: ", STDERR_FILENO);
	ft_putstr_fd(msg_error, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
}

void	*handle_expande_errors(char *msg_error)
{
	handle_errors(msg_error);
	return (NULL);
}
