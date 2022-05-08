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

t_ast	*from_lexer_to_ast(t_list *lst);
t_list *create_ast_list(t_list *lexer); //remplace from_lexer_to_ast

void	exec_ast(t_ast *ast);
void	exec_cmd(t_cmd *node, int *fd);
void	exec_and(t_ast *node);
void	exec_or(t_ast *node);
int		n_redirect(t_list *redirections, int *fd);
void	exec_instruction(t_instruction *node);
void	exec_pipe(t_ast *node);

void	printASTLIST(t_list *ast);
void	printAST(t_ast *ast, int prof);

int		cmd_is_var_attribution(t_cmd *node);
int		char_is_var_attribution(char *str);
void	print_params();
int		get_heredoc(char *limiter);

void	free_ast(void *ast);

#endif