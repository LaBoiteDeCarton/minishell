#include "builtins.h"
#include "minishell.h"

static void	export_valname(char *name)
{
	t_list	*var_ptr;
	char	*new_arg;

	var_ptr = cenv.var;
	while (var_ptr)
	{
		if (!ft_strncmp(((t_var *)(var_ptr->content))->name, name, ft_strlen(name)))
		{
			new_arg = ft_strjoin(name, "=");
			if (!new_arg)
				return (handle_errors("the argument was not exported: "));
			name = ft_strjoin(new_arg, ((t_var *)(var_ptr->content))->value);
			free(new_arg);
			if (!name)
				return (handle_errors("the argument was not exported: "));
			add_to_env(name);
			free(name);
			//ici delete from var utilisant la meme fonction que dans unset please
		}
		var_ptr = var_ptr->next;
	}
}

static void	export_one(char *str)
{
	if (!ft_strchr(str, '='))
		return (export_valname(str));
	if (!char_is_var_attribution(str))
	{
		ft_putstr_fd("msh: export: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		cenv.exit_status = 1;
		return ;
	}
	add_to_env(str);
}

static char	**expande_chartab(char	**chartab)
{
	char	**expanded_chartab;
	int		chartab_len;

	chartab_len = 0;
	while (chartab[chartab_len])
		chartab_len++;
	expanded_chartab = (char **)malloc(sizeof(char *) * (chartab_len + 1));
	expanded_chartab[chartab_len] = NULL;
	while (chartab_len-- > 0)
		expanded_chartab[chartab_len] = expande_char(chartab[chartab_len]);
	return (expanded_chartab);
}

void	exec_builtin_export(t_cmd node, int *fd)
{
	char	**expanded_arg;
	int		i;
	(void)	fd;
	//ici on doit expand chaque arg avant de les executer. (deja expand dan exec_cmd)
	expanded_arg = expande_chartab(node.cmd_arg);
	cenv.exit_status = 0;
	i = 0;
	while (expanded_arg[i])
	{
		export_one(expanded_arg[i]);
		i++;
	}
	free_chartab(expanded_arg);
}