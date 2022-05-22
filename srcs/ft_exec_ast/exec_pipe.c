#include "ast.h"
#include "minishell.h"




/* Sacré bordel... faire du menage dans le pipe please*/
static void	fork_pipe(t_list *content, int fdin)
{
	int status;
	int pid;
	int pipe_fd[2];
	
	if (!content)
		return ;
	if (pipe(pipe_fd) == -1)
		return (handle_errors("pipe"));
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (handle_errors("fork"));
	}
	if (pid == 0)
	{
		if ((close(pipe_fd[0]) == -1)
			|| (fdin != STDIN_FILENO && (dup2(fdin, STDIN_FILENO) == -1 || close(fdin) == -1))
			|| (content->next && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			|| (close(pipe_fd[1]) == -1))
		{
			handle_errors("Pipe");
			exit(258);
		}
		exec_ast((t_ast *)content->content);
		//free des trucs?
		exit(cenv.exit_status);
	}
	if (close(pipe_fd[1]) == -1)
		handle_errors("close");
	cenv.exit_status = 0;
	fork_pipe(content->next, pipe_fd[0]);
	waitpid(pid, &status, 0);
	if (close(pipe_fd[0]) == -1)
		handle_errors("close");
	if (!content->next && WIFEXITED(status))
		cenv.exit_status = WEXITSTATUS(status);
}

void	exec_pipe(t_ast *node)
{
	fork_pipe(node->content, STDIN_FILENO);
}