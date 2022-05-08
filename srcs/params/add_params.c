#include "ast.h"
#include "minishell.h"
#include <stdio.h>

static int	name_is_in_env(char *str)
{
	int		param_name_s;
	int		env_name_s;
	char	**env_ptr;

	param_name_s = 0;
	while (str[param_name_s] && str[param_name_s] != '=')
		param_name_s++;
	env_ptr = cenv.env;
	while (*env_ptr)
	{
		env_name_s = 0;
		while ((*env_ptr)[env_name_s] && (*env_ptr)[env_name_s] != '=')
			env_name_s++;
		if (param_name_s == env_name_s && !ft_strncmp(str, *env_ptr, param_name_s))
			return (1);
		env_ptr++;
	}
	return (0);
}

static void add_to_var(char *str)
{
	t_var	*new_var;
	int		i;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("new variable is not created or modified: ");
		return ;
	}
	i = 0;
	new_var->value = NULL;
	while (str[i] && str[i] != '=')
		i++;
	new_var->name = (char *)malloc(sizeof(char) * i + 1);
	if (!new_var->name)
	{
		free(new_var);
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("new variable is not created or modified: ");
		return ;
	}
	ft_strlcpy(new_var->name, str, i + 1);
	new_var->name[i] = '\0';
	if (str[i])
	{
		i++;
		new_var->value = expande_char(str + i);
		if (!new_var->value)
		{
			free(new_var->name);
			free(new_var);
			ft_putstr_fd("msh: ", STDERR_FILENO);
			perror("new variable is not created or modified: ");
			return ;
		}
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

t_list	*del_from_var(char *name, t_list *var)
{
	t_list	*reste;

	if (!var)
		return (NULL);
	if (!ft_strncmp(name, ((t_var *)var->content)->name, ft_strlen(name) + 1))
	{
		reste = var->next;
		ft_lstdelone(var, &free);
		return (reste);
	}
	var->next = del_from_var(name, var->next);
	return (var);
}

void	add_param(char *param)
{
	if (name_is_in_env(param))
	{
		//printf("%s is in env\n", param);
		add_to_env(param);
	}
	else
	{
		//printf("%s is NOT in env\n", param);
		add_to_var(param);
	}
}

void	del_param(char *name)
{
	del_from_env(name);
	cenv.var = del_from_var(name, cenv.var);
}

void	add_params(char **strtab)
{
	while (*strtab)
	{
		add_param(*strtab);
		strtab++;
	}
}