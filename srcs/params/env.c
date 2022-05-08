#include "minishell.h"
#include <stdio.h>

static void	resize_env()
{
	char	**oldenv;

	oldenv = cenv.env;
	cenv.env = (char **)malloc(sizeof(char *) * (cenv.env_s + 1024));
	if (!cenv.env)
	{
		cenv.env_s = 0;
		ft_putstr_fd("msh: ", STDERR_FILENO);
		perror("environnement parsing error: ");
		return ;
	}
	cenv.env_s += 1024;
	ft_memset(cenv.env, 0, sizeof(char *) * cenv.env_s);
	ft_memcpy(cenv.env, oldenv, sizeof(char *) * cenv.env_s - 1024);
	free(oldenv);
}

static int	cmp_param_name(char *param, char *env_param)
{
	int	i;
	int j;

	i = 0;
	while (param[i] && param[i] != '=')
		i++;
	j=0;
	while (env_param[j] && env_param[j] != '=')
		j++;
	if (i != j)
		return (1);
	return (ft_strncmp(param, env_param, i));
}

void	del_from_env(char *str)
{
	int i;

	i = 0;
	while (i < cenv.env_s)
	{
		if (!cenv.env[i])
			break ;
		if (!cmp_param_name(str, cenv.env[i]))
		{
			free(cenv.env[i]);
			//cenv.env[i] = NULL;
			break ;
		}
		i++;
	}
	while (cenv.env[i])
	{
		cenv.env[i] = cenv.env[i + 1];
		i++;
	}
}

void	add_to_env(char	*str)
{
	int	i;

	i = 0;
	while (cenv.env[i])
	{
		if (i == cenv.env_s - 1)
			resize_env();
		else if (!cmp_param_name(str, cenv.env[i]))
			break;
		i++;
	}
	if (cenv.env[i])
		free(cenv.env[i]);
	cenv.env[i] = expande_char(str);
	if (!cenv.env[i])
		handle_errors("the env is corrupted");
}

void	set_env(char **env)
{
	int i;

	cenv.env = (char **)malloc(sizeof(char *) * 1024);
	if (!cenv.env)
	{
		cenv.env_s = 0;
		return (handle_errors("environnement parsing error"));
	}
	cenv.env_s = 1023;
	i = 0;
	while (i <= cenv.env_s)
		cenv.env[i++] = NULL;
	while (*env)
	{
		add_to_env(*env);
		env++;
	}
	cenv.tcwd = getcwd(NULL, 0);
	if (!cenv.tcwd)
	{
		ft_putstr_fd("msh-init: error retrieving current directory: ", STDERR_FILENO);
		ft_putstr_fd("getcwd: ", STDERR_FILENO);
		perror("cannot access parent directories");
	}
}