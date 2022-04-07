#include "ast.h"
#include "minishell.h"

#include <stdio.h>
static void	print_param(t_var	*var)
{
	printf("VAR\nVAR NAME: %s\nVAR VALUE: %s\n", var->name, var->value);
}

void	print_params()
{
	t_list	*ptr;

	ptr = cenv.var;
	while (ptr)
	{
		print_param((t_var *)ptr->content);
		ptr = ptr->next;
	}
}