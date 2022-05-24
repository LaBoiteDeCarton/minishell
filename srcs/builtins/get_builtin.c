/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:49:12 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:49:13 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_builtin	get_builtin(char *name)
{
	if (!ft_strncmp("echo", name, 5))
		return (bi_echo);
	if (!ft_strncmp("pwd", name, 4))
		return (bi_pwd);
	if (!ft_strncmp("env", name, 4))
		return (bi_env);
	if (!ft_strncmp("export", name, 7))
		return (bi_export);
	if (!ft_strncmp("unset", name, 6))
		return (bi_unset);
	if (!ft_strncmp("exit", name, 5))
		return (bi_exit);
	if (!ft_strncmp("cd", name, 3))
		return (bi_cd);
	return (bi_none);
}
