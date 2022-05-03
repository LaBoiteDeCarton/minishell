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
			return ;
			//ici delete from var utilisant la meme fonction que dans unset please
		}
		var_ptr = var_ptr->next;
	}
	add_to_env(name);
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

//difference entre NULL et /0 dans 

void print_exported(int *fd)
{
	char	**ptr_env;
	char	*ptr_name;

	if (*fd == -1)
		*fd = dup(STDOUT_FILENO);
	if (*fd == -1)
		return (handle_errors("exporte"));
	ptr_env = cenv.env;
	while (*ptr_env)
	{
		ft_putstr_fd("declare -x ", *fd);
		ptr_name = *ptr_env;
		while (*ptr_name && *ptr_name != '=')
		{
			ft_putchar_fd(*ptr_name, *fd);
			ptr_name++;
		}
		if (*ptr_name == '=')
		{
			ptr_name++;
			ft_putstr_fd("=\"", *fd);
			ft_putstr_fd(ptr_name, *fd);
			ft_putchar_fd('"', *fd);
		}
		ft_putendl_fd("", *fd);
		ptr_env++;
	}
}

void	exec_builtin_export(t_cmd node, int *fd)
{
	int		i;
	(void)	fd;

	cenv.exit_status = 0;
	i = 1;
	if (!node.cmd_arg[i])
		return (print_exported(fd + 1));
	while (node.cmd_arg[i])
	{
		export_one(node.cmd_arg[i]);
		i++;
	}
}