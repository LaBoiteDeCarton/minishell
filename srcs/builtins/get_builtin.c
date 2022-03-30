#include "builtins.h"

t_builtin	get_builtin(char *name)
{
	if (!ft_strncmp("echo", name, 5))
		return (bi_echo);
	return bi_none;
}