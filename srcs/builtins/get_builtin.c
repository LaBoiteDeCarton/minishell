#include "builtins.h"

t_builtin	get_builtin(char *name)
{
	if (!ft_strncmp("echo", name, 5))
		return (bi_echo);
	if (!ft_strncmp("pwd", name, 4))
		return (bi_pwd);
	if (!ft_strncmp("env", name, 4))
		return (bi_env);
	if (!ft_strncmp("export", name, 7))
		return (bi_export);
	if (!ft_strncmp("unset", name, 6))
		return (bi_unset);
	if (!ft_strncmp("exit", name, 5))
		return (bi_exit);
	return bi_none;
}