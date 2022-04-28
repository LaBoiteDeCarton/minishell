#include "ast.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>

int	set_stdoutappend(char *path, int *fd)
{
	int		file_fd;
	char	*expanded_path;

	expanded_path = expande_char(path);
	file_fd = open(expanded_path, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (file_fd == -1)
	{
		handle_errors(expanded_path);
		free(expanded_path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'>>': ");
	*fd = file_fd;
	free(expanded_path);
	return (1);
}

int	set_stdout(char *path, int *fd)
{
	int	file_fd;
	char	*expanded_path;

	expanded_path = expande_char(path);
	file_fd = open(expanded_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file_fd == -1)
	{
		handle_errors(expanded_path);
		free(expanded_path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'>'': ");
	*fd = file_fd;
	free(expanded_path);
	return (1);
}

int	set_stdin(char *path, int *fd)
{
	int	file_fd;
	char	*expanded_path;

	expanded_path = expande_char(path);
	file_fd = open(expanded_path, O_RDONLY);
	if (file_fd == -1)
	{
		handle_errors(expanded_path);
		free(expanded_path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'<': ");
	*fd = file_fd;
	free(expanded_path);
	return (1);
}

int	set_stdinheredoc(int heredoc_fd, int *fd)
{
	if (heredoc_fd == -1)
		return (0);
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'<<': ");
	*fd = heredoc_fd;
	return (1);
}

static int	redirect(t_redirect redirection, int *fd)
{
	if (redirection.red_type == red_in)
		return (set_stdin(redirection.pathfile, fd));
	else if (redirection.red_type == red_out)
		return (set_stdout(redirection.pathfile, fd + 1));
	else if (redirection.red_type == red_append)
		return (set_stdoutappend(redirection.pathfile, fd + 1));
	else if (redirection.red_type == red_heredoc)
		return (set_stdinheredoc(redirection.fd, fd));
	return (0);
}

int	n_redirect(t_list *redirections, int *fd)
{
	t_list *ptr;

	ptr = redirections;
	while (ptr)
	{
		if (!redirect(*(t_redirect *)ptr->content, fd))
			return (0);
		ptr = ptr->next;
	}
	return (1);
}