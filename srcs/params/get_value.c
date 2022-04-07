#include "minishell.h"

static char	*get_from_var(char *name, int name_s)
{
	t_list	*var_ptr;

	var_ptr = cenv.var;
	while (var_ptr)
	{
		if (!ft_strncmp(name, ((t_var *)var_ptr->content)->name, name_s))
			return (((t_var *)var_ptr->content)->value);
		var_ptr = var_ptr->next;
	}
	return (NULL);
}

static char	*get_from_env(char *name, int name_s)
{
	char	**env_ptr;

	env_ptr = cenv.env;
	while (*env_ptr)
	{
		if (!ft_strncmp(name, *env_ptr, name_s) && (*env_ptr)[name_s] == '=')
			return ((*env_ptr) + (name_s + 1));
		env_ptr++;
	}
	return (NULL);
}

int	var_name_size(char *name)
{
	int	name_size;

	name_size = 0;
	if (name[name_size] && ft_isdigit(name[name_size]))
		return (1);
	while (name[name_size] && ft_isalnum(name[name_size]))
			name_size++;
	return (name_size);
}

//on retourne un pointeur sur le premier caractere de la valeur (pas de copie)
char	*get_value(char *name)
{
	char	*value;
	int		name_size;

	name_size = var_name_size(name);
	value = get_from_env(name, name_size);
	if (!value)
		value = get_from_var(name, name_size);
	return (value);
}
