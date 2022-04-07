#include "minishell.h"
#include <stdio.h>

void	handle_errors(char *msg_error)
{
	ft_putstr_fd("msh: ", STDERR_FILENO);
	perror(msg_error);
}