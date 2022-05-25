/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:38 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:39 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

static void	redirect_error(void)
{
	handle_errors("Pipe");
	exit(258);
}

static void	child(t_list *content, int *pipe_fd, int fdin)
{
	if (close(pipe_fd[0]) == -1)
		redirect_error();
	if ((fdin != STDIN_FILENO && (dup2(fdin, STDIN_FILENO) == -1))
		|| close(fdin) == -1)
		redirect_error();
	if (content->next && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		redirect_error();
	if (close(pipe_fd[1]) == -1)
		redirect_error();
	exec_ast((t_ast *)content->content);
	msh_exit();
}

static void	close_pipe(int *pipe_fd)
{
	if (close(pipe_fd[0]) == -1)
		handle_errors("close");
	if (close(pipe_fd[1]) == -1)
		handle_errors("close");
}

static void	fork_pipe(t_list *content, int fdin)
{
	int	status;
	int	pid;
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (handle_errors("pipe"));
	pid = fork();
	if (pid == -1)
	{	
		close_pipe(pipe_fd);
		g_cenv.exit_status = 128;
		return (handle_errors("fork"));
	}
	if (pid == 0)
		child(content, pipe_fd, fdin);
	if (close(pipe_fd[1]) == -1)
		handle_errors("close");
	g_cenv.exit_status = 0;
	if (content->next)
		fork_pipe(content->next, pipe_fd[0]);
	waitpid(pid, &status, 0);
	if (close(pipe_fd[0]) == -1)
		handle_errors("close");
	if (!content->next && WIFEXITED(status))
		g_cenv.exit_status = WEXITSTATUS(status);
}

void	exec_pipe(t_ast *node)
{
	fork_pipe(node->content, STDIN_FILENO);
}
