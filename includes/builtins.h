#ifndef BUILTINS_H
# define BUILTINS_H

#include "ast.h"

typedef enum s_builtin
{
	bi_none,
	bi_echo,
	bi_cd,
	bi_pwd,
	bi_export,
	bi_unset,
	bi_env,
	bi_exit
} t_builtin;

t_builtin	get_builtin(char *name);
void		exec_builtin(t_cmd node, int *fd);
void		exec_builtin_echo(t_cmd node, int *fd);
void		exec_builtin_cd(t_cmd node, int *fd);

#endif