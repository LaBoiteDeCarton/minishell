#include "ast.h"
#include "minishell.h"

int	get_heredoc(char *limiter)
{
	int		pipe_fd[2];
	char	*line;
	char	*expanded_line;
	int		status;
	pid_t	pid_id;

	if (pipe(pipe_fd))
	{
		handle_errors(limiter);
		return (-1);
	}
	pid_id = fork();
	if (pid_id == -1)
	{
		handle_errors(limiter);
		return (-1);
	}
	if (pid_id == 0)
	{
		init_heredoc_signal();
		close(pipe_fd[0]);
		while (1)
		{
			line = ft_readline("> ");
			if (!line)
				break ;
			if (line && !ft_strncmp(limiter, line, ft_strlen(limiter) + 1))
			{
				free(line);
				break ;
			}
			expanded_line = expande_char(line);
			ft_putstr_fd(expanded_line, pipe_fd[1]);
			ft_putstr_fd("\n", pipe_fd[1]);
			free(expanded_line);
			free(line);
		}
		close(pipe_fd[1]);
		exit(EXIT_SUCCESS);
	}
	signal(SIGINT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid_id, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		cenv.exit_status = 255;
		return (-1);
	}
	init_signals();
	return (pipe_fd[0]);
}