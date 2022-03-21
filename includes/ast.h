#ifndef AST_H
# define AST_H
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>

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
	null,
	cmd,
	redirection,
	instruction,
	and,
	or,
	pipe
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
	t_redirect	*redirect;
	t_cmd		cmd;
};

// peut etre pas besoin de t_builtin a voir si plus co;plique ou plus legers
struct s_builtin 
{
	char	*cmd_name;
	char	**cmd_arg;
};

typedef enum s_redirect_type
{
	null,
	in,
	out,
	heredoc,
	append
}	t_redirect_type;

struct s_redirect
{
	t_redirect_type	red_type;
	char			*pathfile;
	char			*end;
};

struct s_pipe
{
	t_ast	**content;
};

struct s_or
{
	t_ast	**content;
};

struct s_and
{
	t_ast	**content;
};

int	exec_cmd(t_cmd node);
int	n_redirect(t_redirect *redirections);
void	exec_instruction(t_instruction node);

#endif