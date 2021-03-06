/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:48:49 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:48:50 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	exec_builtin(t_cmd node, int *fd)
{
	t_builtin	bi_type;

	bi_type = get_builtin(node.cmd_name);
	if (bi_type == bi_echo)
		exec_builtin_echo(node, fd);
	if (bi_type == bi_pwd)
		exec_builtin_pwd(node, fd);
	if (bi_type == bi_env)
		exec_builtin_env(node, fd);
	if (bi_type == bi_export)
		exec_builtin_export(node, fd);
	if (bi_type == bi_unset)
		exec_builtin_unset(node, fd);
	if (bi_type == bi_exit)
		exec_builtin_exit(node, fd);
	if (bi_type == bi_cd)
		exec_builtin_cd(node, fd);
}
