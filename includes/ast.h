/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:29:02 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:29:04 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

typedef struct s_ast			t_ast;
typedef struct s_instruction	t_instruction;
typedef struct s_cmd			t_cmd;
typedef struct s_redirect		t_redirect;
typedef struct s_and			t_and;
typedef struct s_or				t_or;
typedef struct s_pipe			t_pipe;

typedef enum s_token
{
	token_execute,
	token_instruction,
	token_and,
	token_or,
	token_pipe,
	token_error
}	t_token;

struct s_ast
{
	t_token	token;
	t_list	*content;
};

struct s_cmd
{
	char		*cmd_name;
	char		**cmd_arg;
};

struct s_instruction
{
	t_list		*redirection;
	t_cmd		*cmd;
	int			fd[2];
};

typedef enum s_redirect_type
{
	red_in,
	red_out,
	red_heredoc,
	red_append,
	red_error
}	t_redirect_type;

struct s_redirect
{
	t_redirect_type	red_type;
	int				fd;
	char			*pathfile;
};

/*
	Functions used to create the AST tree
*/
t_list			*create_ast_lst(t_list *lexer);
t_ast			*next_ast(t_list **lexer, int piping);
t_ast			*from_lexer_to_ast(t_list *lst);
t_ast			*from_lexer_to_pipe(t_list *lexer);
t_instruction	*from_lexer_to_instruction(t_list *lexer);
t_redirect_type	get_red(t_list *lst);
int				get_heredoc(char *limiter);
t_token			translate_token(t_list *lexer);

/*
	Function used to Execute the AST tree recursively
*/
void			exec_ast_lst(t_list *ast_lst);
void			exec_ast(t_ast *ast);
void			exec_and(t_ast *node);
void			exec_or(t_ast *node);
void			exec_pipe(t_ast *node);
void			exec_instruction(t_instruction *node);
int				redirect(t_redirect redirection, int *fd);
void			exec_cmd(t_cmd *node, int *fd);
char			*find_path(char *f);
int				cmd_is_var_attribution(t_cmd *node);
int				char_is_var_attribution(char *str);

/*
	AST recursive free function
*/
void			free_ast(void *ast);

#endif
