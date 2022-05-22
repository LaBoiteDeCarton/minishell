#include "minishell.h"
#include <stdio.h>

void	handle_errors(char *msg_error)
{
	ft_putstr_fd("msh: ", STDERR_FILENO);
	ft_putstr_fd(msg_error, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
}