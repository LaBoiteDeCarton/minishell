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
		handle_errors(path);
		return (0);
	}
	return (1);
}

int	set_stdout(char *path, int *fd)
{
	if (*fd > 0)
		close(*fd);
	*fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (*fd == -1)
	{
		handle_errors(path);
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
		handle_errors(path);
		return (0);
	}
	return (1);
}

int	set_stdinheredoc(char *limiter, int *fd) //ICI ON DOIT FORK POUR EVITER QUE ^D ne coupe stdin du main
{
	int		pipe_fd[2];
	char	*line;
	int		status;
	pid_t	pid_id;

	int		fd_stdin;
	fd_stdin = dup(STDIN_FILENO);
	if (*fd > 0)
		close(*fd);
	if (pipe(pipe_fd))
	{
		handle_errors(limiter);
		return (0);
	}
	pid_id = fork();
	if (pid_id == -1)
	{
		handle_errors(limiter);
		return (0);
	}
	if (pid_id == 0)
	{
		close(pipe_fd[0]);
		while (1)
		{
			line = ft_readline("> ");
			if (!line)
				break ;
			if (line && !ft_strncmp(limiter, line, ft_strlen(limiter) + 1)) //len + 1? pour prendre en compte le NULL?
			{
				free(line);
				break ;
			}
			ft_putstr_fd(line, pipe_fd[1]);
			ft_putstr_fd("\n", pipe_fd[1]);
			free(line);
		}
		close(pipe_fd[1]);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[1]);
	waitpid(pid_id, &status, 0);
	dup2(fd_stdin, STDIN_FILENO);
	dup2(pipe_fd[0], *fd);
	close(pipe_fd[0]);
	if (WIFSIGNALED(status))
		return (0);
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