#include "ast.h"
#include <fcntl.h>
#include <unistd.h>

int	set_stdout(char *path)
{
	int fd_out;

	fd_out = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
	{
		write(2, "Unable to open\n", 15);
		return (0);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		write(2, "Unable to dup2\n", 15);
		return (0);
	}
	close(fd_out);
	return (1);
}

int	set_stdin(char *path)
{
	int fd;
	
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Unable to open\n", 15); // differents types dérrer avec strerrno
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		write(2, "Unable to dup2\n", 15);
		return (0);
	}
	close(fd);
	return (1);
}

static int	redirect(t_redirect redirection)
{
	if (redirection.red_type == red_in)
		return (set_stdin(redirection.pathfile));
	else if (redirection.red_type == red_out)
		return (set_stdout(redirection.pathfile));
	return (0);
}

int	n_redirect(t_list *redirections)
{
	t_list *ptr;

	ptr = redirections;
	while (ptr)
	{
		if (!redirect(*(t_redirect *)ptr->content))
			return (0);
		ptr = ptr->next;
	}
	return (1);
}