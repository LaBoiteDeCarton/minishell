/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:50:29 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:50:30 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

static void	export_valname(char *str)
{
	t_list	*v_ptr;
	char	*new_arg;

	v_ptr = g_cenv.var;
	while (v_ptr)
	{
		if (!ft_strncmp(((t_var *)(v_ptr->content))->name, str, ft_strlen(str)))
		{
			new_arg = ft_strjoin(str, "=");
			if (!new_arg)
				return (handle_errors("the argument was not exported: "));
			str = ft_strjoin(new_arg, ((t_var *)(v_ptr->content))->value);
			free(new_arg);
			if (!str)
				return (handle_errors("the argument was not exported: "));
			add_to_env(str);
			free(str);
			return ;
		}
		v_ptr = v_ptr->next;
	}
	add_to_env(str);
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
		g_cenv.exit_status = 1;
		return ;
	}
	add_to_env(str);
}

static void	print_exported(int *fd)
{
	char	**ptr_env;
	char	*ptr_name;

	ptr_env = g_cenv.env;
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
	int	i;

	if (*fd == -1)
		*fd = dup(STDOUT_FILENO);
	if (*fd == -1)
		return (handle_errors("exporte"));
	g_cenv.exit_status = 0;
	i = 1;
	if (!node.cmd_arg[i])
		return (print_exported(fd + 1));
	while (node.cmd_arg[i])
	{
		export_one(node.cmd_arg[i]);
		i++;
	}
}
