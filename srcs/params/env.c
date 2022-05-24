/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:11:21 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 16:11:22 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	resize_env(void)
{
	char	**oldenv;

	oldenv = g_cenv.env;
	g_cenv.env = (char **)malloc(sizeof(char *) * (g_cenv.env_s + 1024));
	if (!g_cenv.env)
	{
		g_cenv.env_s = 0;
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("environnement parsing error: ");
		return ;
	}
	g_cenv.env_s += 1024;
	ft_memset(g_cenv.env, 0, sizeof(char *) * g_cenv.env_s);
	ft_memcpy(g_cenv.env, oldenv, sizeof(char *) * g_cenv.env_s - 1024);
	free(oldenv);
}

static int	cmp_param_name(char *param, char *env_param)
{
	int	i;
	int	j;

	i = 0;
	while (param[i] && param[i] != '=')
		i++;
	j = 0;
	while (env_param[j] && env_param[j] != '=')
		j++;
	if (i != j)
		return (1);
	return (ft_strncmp(param, env_param, i));
}

void	del_from_env(char *str)
{
	int	i;

	i = 0;
	while (i < g_cenv.env_s)
	{
		if (!g_cenv.env[i])
			break ;
		if (!cmp_param_name(str, g_cenv.env[i]))
		{
			free(g_cenv.env[i]);
			break ;
		}
		i++;
	}
	while (g_cenv.env[i])
	{
		g_cenv.env[i] = g_cenv.env[i + 1];
		i++;
	}
}

void	add_to_env(char	*str)
{
	int	i;

	i = 0;
	while (g_cenv.env[i])
	{
		if (i == g_cenv.env_s - 1)
			resize_env();
		else if (!cmp_param_name(str, g_cenv.env[i]))
			break ;
		i++;
	}
	if (g_cenv.env[i])
		free(g_cenv.env[i]);
	g_cenv.env[i] = expande_char(str);
	if (!g_cenv.env[i])
		handle_errors("the env is corrupted");
}

void	set_env(char **env)
{
	int	i;

	g_cenv.env = (char **)malloc(sizeof(char *) * 1024);
	if (!g_cenv.env)
	{
		g_cenv.env_s = 0;
		return (handle_errors("environnement parsing error"));
	}
	g_cenv.env_s = 1023;
	i = 0;
	while (i <= g_cenv.env_s)
		g_cenv.env[i++] = NULL;
	while (*env)
	{
		add_to_env(*env);
		env++;
	}
	g_cenv.tcwd = getcwd(NULL, 0);
	if (!g_cenv.tcwd)
	{
		ft_putstr_fd("msh-init: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: ", STDERR_FILENO);
		perror("cannot access parent directories");
	}
}
