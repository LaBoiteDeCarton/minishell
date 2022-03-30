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
	if (node.content->next && pipe(pipe_fd) == -1)
		return ; //ici erreur de creation de pipe avec variable ERRNO!
	pid = fork();
	if (pid == -1)
		return ; //ici erreur de creation de fork avec variable ERRNO;
	if (pid == 0)
	{
		//child
		if (fdin != STDIN_FILENO)
		{
			dup2(fdin, STDIN_FILENO);
			close(fdin);
		}
		if (node.content->next)
		{
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
		}
		exec_ast(*(t_ast *)node.content->content);
		//free des trucs?
		exit(cenv.exit_status);
	}
	if (node.content->next)
	{
		close(pipe_fd[1]);
		node.content = node.content->next;
		fork_pipe(node, pipe_fd[0]);
	}
	waitpid(pid, &status, 0);
	if (node.content->next)
		close(pipe_fd[0]);
	cenv.exit_status = WEXITSTATUS(status);
}

void	exec_pipe(t_ast node)
{
	fork_pipe(node, STDIN_FILENO);
}