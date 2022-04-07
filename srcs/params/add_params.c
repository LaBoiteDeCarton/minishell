#include "ast.h"
#include "minishell.h"
#include <stdio.h>

static int	name_is_in_env(char *str)
{
	int		param_name_s;
	char	**env_ptr;

	param_name_s = 0;
	while (str[param_name_s] && str[param_name_s] != '=')
		param_name_s++;
	if (str[param_name_s] == '=')
		param_name_s++;
	env_ptr = cenv.env;
	while (*env_ptr)
	{
		if (!ft_strncmp(str, *env_ptr, param_name_s))
			return (1);
		env_ptr++;
	}
	return (0);
}

static void add_to_var(char *str)
{
	t_var	*new_var;
	char	*ptr;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("new variable is not created or modified: ");
		return ;
	}
	ptr = ft_strchr(str, '=');
	*ptr = '\0';
	ptr++;
	new_var->name = ft_strdup(str);
	if (!new_var->name)
	{
		free(new_var);
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("new variable is not created or modified: ");
		return ;
	}
	new_var->value = ft_strdup(ptr); //ici expand d'abord le ptr
	if (!new_var->value)
	{
		free(new_var->name);
		free(new_var);
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("new variable is not created or modified: ");
		return ;
	}
	//if (errno?)
	ft_lstadd_front(&cenv.var, ft_lstnew(new_var));
	// var_ptr = cenv.var; supprimer le doublon
	// while (var_ptr)
	// {
	// 	if (var_ptr->next)
	// 	{
	// 		if (!ft_strncmp(((t_var *)var_ptr->next->content)->name, new_var->name, ft_strlen(new_var->name))

	// 	}
	// 	var_ptr = var_ptr->next;
	// }
}

void	add_params(char **strtab)
{
	while (*strtab)
	{
		if (name_is_in_env(*strtab))
			add_to_env(*strtab);
		else
			add_to_var(*strtab);
		strtab++;
	}
}