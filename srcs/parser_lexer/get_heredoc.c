/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:59:04 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:59:05 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

static void	child(int *pipe_fd, char *limiter)
{
	char	*line;
	char	*expanded_line;

	init_heredoc_signal();
	close(pipe_fd[0]);
	while (1)
	{
		line = ft_readline("> ");
		if (!line
			|| (line && !ft_strncmp(limiter, line, ft_strlen(limiter) + 1)))
			break ;
		expanded_line = expande_char(line);
		ft_putstr_fd(expanded_line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(expanded_line);
		free(line);
	}
	if (line)
		free(line);
	close(pipe_fd[1]);
	free_chartab(g_cenv.env);
	free_var(g_cenv.var);
	if (g_cenv.ast)
		ft_lstclear(&g_cenv.ast, &free_ast);
	exit(EXIT_SUCCESS);
}

static int	handle_heredoc_errors(char *limiter)
{
	handle_errors(limiter);
	return (-1);
}

static void	close_pipe(int *pipe_fd)
{
	if (close(pipe_fd[0]) == -1)
		handle_errors("close");
	if (close(pipe_fd[1]) == -1)
		handle_errors("close");
}

int	get_heredoc(char *limiter)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid_id;

	if (pipe(pipe_fd))
		return (handle_heredoc_errors(limiter));
	pid_id = fork();
	if (pid_id == -1)
	{
		close_pipe(pipe_fd);
		return (handle_heredoc_errors(limiter));
	}
	if (pid_id == 0)
		child(pipe_fd, limiter);
	signal(SIGINT, SIG_IGN);
	close(pipe_fd[1]);
	waitpid(pid_id, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		g_cenv.exit_status = 255;
		return (-1);
	}
	init_signals();
	return (pipe_fd[0]);
}
