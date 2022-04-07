#include "ast.h"

int	char_is_var_attribution(char *str)
{
	if (!str || !*str || !ft_isalpha(*str))
		return (0);
	while (ft_isalnum(*str))
		str++;
	if (!*str || *str != '=')
		return (0);
	return (1);
}

int	cmd_is_var_attribution(t_cmd node)
{
	while (*node.cmd_arg)
	{
		if (!char_is_var_attribution(*node.cmd_arg))
			return (0);
		node.cmd_arg++;
	}
	return (1);
}
