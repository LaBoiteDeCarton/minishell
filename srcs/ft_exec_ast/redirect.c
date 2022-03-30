#include "ast.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int	set_stdoutappend(char *path, int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (*fd == -1)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror(path);
		return (0);
	}
	return (1);
}

int	set_stdout(char *path, int *fd)
{
	*fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (*fd == -1)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror(path);
		return (0);
	}
	return (1);
}

int	set_stdin(char *path, int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = open(path, O_RDONLY);
	if (*fd == -1)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror(path);
		return (0);
	}
	return (1);
}

int	set_stdinheredoc(char *limiter, int *fd)
{
	int		pipe_fd[2];
	char	*line;

	if (*fd > 0)
		close(*fd);
	if (pipe(pipe_fd))
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror(limiter);
		return (0);
	}
	ft_putstr_fd("> ", STDOUT_FILENO);
	while (get_next_line(0, &line))
	{
		if (line && !ft_strncmp(limiter, line, ft_strlen(limiter) + 1)) //len + 1? pour prendre en compte le NULL?
		{
			free(line);
			break;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
		ft_putstr_fd("> ", STDOUT_FILENO);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], *fd);
	close(pipe_fd[0]);
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
		return (set_stdinheredoc(redirection.pathfile, fd));
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