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

static void	redirect_error(char *str)
{
	handle_errors(str);
	exit(258);
}

static void	child(t_list *content, int *pipe_fd, int fdin)
{
	if (close(pipe_fd[0]) == -1)
		redirect_error("Pipe: close");
	if (dup2(fdin, STDIN_FILENO) == -1)
		redirect_error("Pipe: dup2");
	if (fdin != STDIN_FILENO && close(fdin) == -1)
		redirect_error("Pipe: close");
	if (content->next && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		redirect_error("Pipe: dup2");
	if (close(pipe_fd[1]) == -1)
		redirect_error("Pipe: close");
	exec_ast((t_ast *)content->content);
	free_chartab(g_cenv.env);
	free_var(g_cenv.var);
	rl_clear_history();
	if (g_cenv.ast)
		ft_lstclear(&g_cenv.ast, &free_ast);
	exit(g_cenv.exit_status);
}

static void	close_pipe(int *pipe_fd)
{
	if (close(pipe_fd[0]) == -1)
		handle_errors("pipe: close");
	if (close(pipe_fd[1]) == -1)
		handle_errors("pipe: close");
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
	if (close(pipe_fd[0]) == -1)
		handle_errors("close");
	waitpid(pid, &status, 0);
	if (!content->next && WIFEXITED(status))
		g_cenv.exit_status = WEXITSTATUS(status);
}

void	exec_pipe(t_ast *node)
{
	fork_pipe(node->content, STDIN_FILENO);
}
