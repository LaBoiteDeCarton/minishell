#include "ast.h"
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	cenv = env;
}
