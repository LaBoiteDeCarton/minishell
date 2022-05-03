#include "builtins.h"
#include "minishell.h"
#include <sys/stat.h>

/*
	BUILT-IN CD
	How it works :
		do chdir:
			if return (-1) - the directory doesn't exist
			if return (0) - we do the maj of the tcwd (The Current Work Directory) value
	checker si path existe
		tel quel, puis accroché au pwd (getcwd)

	Si oui et OLDPWD existe mettre la variable env OLDPWD avec la valeur du current PWD
	Si oui et PWD existe mettre la variable env PWD avec la valeur du nouveau PWD
*/

static void maj_tcwd(char *path)
{
	char	*new_tcwd;
	
	new_tcwd = getcwd(NULL, 0);
	if (!new_tcwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", STDERR_FILENO);
		ft_putstr_fd("getcwd: ", STDERR_FILENO);
		perror("cannot access parent directories");
		path = ft_strjoin("/", path);
		new_tcwd = ft_strjoin(cenv.tcwd, path);
		if (path)
			free(path);
	}
	if (cenv.tcwd)
		free(cenv.tcwd);
	cenv.tcwd = new_tcwd;
}

void	exec_builtin_cd(t_cmd node, int *fd)
{
	(void)node;
	(void)fd;

	if (chdir(node.cmd_arg[1]) == -1)
	{
		cenv.exit_status = 1;
		handle_errors(node.cmd_arg[1]);
	}
	else
	{
		cenv.exit_status = 0;
		maj_tcwd(node.cmd_arg[1]);
	}
}
//on fait le chdir, si ko voilà.
//si ok on essaye de creer le nouveau pwd en faiant appel a getcwd si possible,
	//si ko alors petit souci on l'affiche, puis on continue (sans erreur car chdir a fonctionné!!)
//on concatene avec le tcwd sauvegardé, maj + maj de OLDPWD et PWD (si existe)
	
/*
!args[1] || ft_strequ(args[1], "~") 

home unset error "cd: HOME not set"
*/