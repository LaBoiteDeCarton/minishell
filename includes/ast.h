#ifndef AST_H
# define AST_H
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

char	**cenv;

typedef struct s_ast			t_ast;
typedef struct s_instruction	t_instruction;
typedef struct s_cmd			t_cmd;
typedef struct s_builtin		t_builtin;
typedef struct s_redirect		t_redirect;
typedef struct s_and			t_and;
typedef struct s_or				t_or;
typedef struct s_pipe			t_pipe;

typedef enum s_token
{
	token_cmd,
	token_instruction,
	token_and,
	token_or,
	token_pipe,
	token_error
}	t_token;

struct s_ast
{
	t_token	token;
	void	*content;
};

struct s_cmd
{
	char	*cmd_name;
	char	**cmd_arg;
};

struct s_instruction
{
	t_list		*redirection;
	t_cmd		*cmd;
};

typedef enum s_redirect_type
{
	red_in,
	red_out,
	red_heredoc,
	red_append
}	t_redirect_type;

struct s_redirect
{
	t_redirect_type	red_type;
	char			*pathfile;
	char			*end;
};

struct s_pipe
{
	t_list	*content;
};

struct s_or
{
	t_list	*content;
};

struct s_and
{
	t_list	*content;
};

int	exec_ast(t_ast ast);
int	exec_cmd(t_cmd node);
int	exec_and(t_and node);
int	exec_or(t_or node);
int	n_redirect(t_list *redirections);
int	exec_instruction(t_instruction node);
int	exec_pipe(t_pipe node);

#endif