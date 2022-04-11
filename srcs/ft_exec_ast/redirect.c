#include "ast.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>

int	set_stdoutappend(char *path, int *fd)
{
	int	file_fd;

	file_fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (file_fd == -1)
	{
		handle_errors(path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("heredoc: ");
	*fd = file_fd;
	return (1);
}

int	set_stdout(char *path, int *fd)
{
	int	file_fd;

	file_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file_fd == -1)
	{
		handle_errors(path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("heredoc: ");
	*fd = file_fd;
	return (1);
}

int	set_stdin(char *path, int *fd)
{
	int	file_fd;

	file_fd = open(path, O_RDONLY);
	if (file_fd == -1)
	{
		handle_errors(path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("stdin: ");
	*fd = file_fd;
	return (1);
}

// REAL HEREDOC IS HERE
int	set_stdinheredoc(int heredoc_fd, int *fd)
{
	if (heredoc_fd == -1)
		return (0);
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("heredoc: ");
	*fd = heredoc_fd;
	return (1);
}

// int	set_stdinheredoc(char *limiter, int *fd)
// {
// 	int		pipe_fd[2];
// 	char	*line;
// 	int		status;
// 	pid_t	pid_id;

// 	if (*fd > 0)
// 		close(*fd);
// 	if (pipe(pipe_fd))
// 	{
// 		handle_errors(limiter);
// 		return (0);
// 	}
// 	pid_id = fork();
// 	if (pid_id == -1)
// 	{
// 		handle_errors(limiter);
// 		return (0);
// 	}
// 	if (pid_id == 0)
// 	{
// 		init_heredoc_signal();
// 		close(pipe_fd[0]);
// 		while (1)
// 		{
// 			line = ft_readline("> ");
// 			if (!line)
// 				break ;
// 			if (line && !ft_strncmp(limiter, line, ft_strlen(limiter) + 1)) //len + 1? pour prendre en compte le NULL?
// 			{
// 				free(line);
// 				break ;
// 			}
// 			ft_putstr_fd(line, pipe_fd[1]);
// 			ft_putstr_fd("\n", pipe_fd[1]);
// 			free(line);
// 		}
// 		close(pipe_fd[1]);
// 		exit(EXIT_SUCCESS);
// 	}
// 	signal(SIGINT, SIG_IGN);
// 	close(pipe_fd[1]);
// 	waitpid(pid_id, &status, 0);
// 	init_signals();
// 	*fd = pipe_fd[0];
// 	if (WIFSIGNALED(status))
// 		return (0);
// 	return (1);
// }

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