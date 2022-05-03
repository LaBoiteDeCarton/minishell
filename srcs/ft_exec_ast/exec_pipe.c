#include "ast.h"
#include "minishell.h"

/* Sacré bordel... faire du menage dans le pipe please*/
void	fork_pipe(t_ast node, int fdin)
{
	int status;
	int pid;
	int pipe_fd[2];
	
	if (!node.content)
		return ;
	if (pipe(pipe_fd) == -1)
		return (handle_errors("pipe"));
	pid = fork();
	if (pid == -1)
		return (handle_errors("fork"));
	if (pid == 0)
	{
		if ((close(pipe_fd[0]) == -1)
			|| (fdin != STDIN_FILENO && (dup2(fdin, STDIN_FILENO) == -1 || close(fdin) == -1))
			|| (node.content->next && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			|| (close(pipe_fd[1]) == -1))
		{
			handle_errors("Pipe");
			exit(258);
		}
		exec_ast(*(t_ast *)node.content->content);
		//free des trucs?
		exit(cenv.exit_status);
	}
	if (close(pipe_fd[1]) == -1)
		handle_errors("close");
	node.content = node.content->next;
	if (node.content)
		fork_pipe(node, pipe_fd[0]);
	waitpid(pid, &status, 0);
	if (close(pipe_fd[0]) == -1)
		handle_errors("close");
	if (WIFEXITED(status))
		cenv.exit_status = WEXITSTATUS(status);
	else
		cenv.exit_status = 13;
}

void	exec_pipe(t_ast node)
{
	fork_pipe(node, STDIN_FILENO);
}