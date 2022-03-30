#ifndef AST_H
# define AST_H
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_instruction	t_instruction;
typedef struct s_cmd			t_cmd;
typedef struct s_builtin		t_builtin;
typedef struct s_redirect		t_redirect;
typedef struct s_node			t_node;

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

typedef enum s_node_type
{
	null,
	and,
	or,
	pipe,
	cmd,
	instruction,
	redirection
} t_node_type;

struct s_node
{
	t_node_type node_type;
	t_node *node;
	t_instruction *instruction;
};

int	exec_cmd(t_cmd node);
int	n_redirect(t_redirect *redirections);
void	exec_instruction(t_instruction node);

#endif