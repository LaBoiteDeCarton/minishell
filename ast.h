#ifndef AST_H
# define AST_H

typedef struct s_instruction	t_instruction;
typedef struct s_cmd			t_cmd;
typedef struct s_redirect		t_redirect;
typedef struct s_pipe			t_pipe;
typedef struct s_and			t_and;
typedef struct s_or				t_or;

struct s_instruction
{
	t_redirect	*redirect; // a noter quón peut avoir plusieurs redirections, < file1 < file2 cat = va afficher les deux fichiers
	t_cmd		*t_cmd;
};

struct s_cmd
{
	char	*cmd_name;
	char	**cmd_arg;
};

typedef enum s_redirect_type
{
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
	t_instruction	*instructions;
};

struct	s_and
{
	t_instruction left;
	t_instruction right;
};

struct s_or
{
	t_instruction left;
	t_instruction right;
};

#endif