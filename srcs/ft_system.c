#include "minishell.h"
#include <stdlib.h>

void	ft_system(char *command)
{
	system(command);

	// lst = create_lexer(commande);
	// printf_lexer(lst);
	// ft_lstclear(&lst, &free_content);
	// free(commande);
}