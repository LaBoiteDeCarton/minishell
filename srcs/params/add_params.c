/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:11:16 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 16:11:17 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

static int	name_is_in_env(char *str)
{
	int		param_name_s;
	int		env_name_s;
	char	**env_ptr;

	param_name_s = 0;
	while (str[param_name_s] && str[param_name_s] != '=')
		param_name_s++;
	env_ptr = g_cenv.env;
	while (*env_ptr)
	{
		env_name_s = 0;
		while ((*env_ptr)[env_name_s] && (*env_ptr)[env_name_s] != '=')
			env_name_s++;
		if (param_name_s == env_name_s
			&& !ft_strncmp(str, *env_ptr, param_name_s))
			return (1);
		env_ptr++;
	}
	return (0);
}

static void	handle_add_to_var_error(t_var *new_var)
{
	ft_putstr_fd("msh: ", STDERR_FILENO);
	perror("new variable is not created or modified: ");
	if (new_var && new_var->name)
		free(new_var->name);
	if (new_var && new_var->value)
		free(new_var->value);
	if (new_var)
		free(new_var);
}

static void	add_to_var(char *str)
{
	t_var	*new_var;
	int		i;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
		return (handle_add_to_var_error(new_var));
	i = 0;
	new_var->value = NULL;
	while (str[i] && str[i] != '=')
		i++;
	new_var->name = ft_substr(str, 0, i);
	if (!new_var->name)
		return (handle_add_to_var_error(new_var));
	new_var->name[i] = '\0';
	if (str[i])
	{
		i++;
		new_var->value = expande_char(str + i);
		if (!new_var->value)
			return (handle_add_to_var_error(new_var));
	}
	ft_lstadd_front(&g_cenv.var, ft_lstnew(new_var));
}

void	add_param(char *param)
{
	if (name_is_in_env(param))
		add_to_env(param);
	else
		add_to_var(param);
}

void	add_params(char **strtab)
{
	while (*strtab)
	{
		add_param(*strtab);
		strtab++;
	}
}
