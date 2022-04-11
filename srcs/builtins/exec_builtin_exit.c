#include "builtins.h"
#include "minishell.h"
#include <stdlib.h>

static int	ft_strisll(char *str)
{
	char	*ptr;

	if (str && (*str == '-'|| *str == '+'))
		str++;
	ptr = str;
	while (str && *str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	if (ft_strlen(ptr) > 19)
		return (0);
	if (ft_strlen(ptr) == 19)
		return (!(ft_strncmp(ptr, "9223372036854775807", 20) >= 0));
	return (1);
}

static int	ft_atoll(const char *str)
{
	long long int	res;
	int				signe;

	res = 0;
	signe = 1;
	while (*str == ' ' || *str == '\r' || *str == '\n'
		|| *str == '\f' || *str == '\v' || *str == '\t')
		str++;
	if (*str == '-')
		signe = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (ft_isdigit(*str))
	{
		res = res * 10 + signe * (*str - '0');
		str++;
	}
	return (res);
}

void	exec_builtin_exit(t_cmd node, int *fd)
{
	(node.cmd_arg)++;
	if (*node.cmd_arg)
	{
		if (ft_strisll(*node.cmd_arg))
			cenv.exit_status = ft_atoll(*node.cmd_arg);
		else
		{
			ft_putstr_fd("msh: exit: ", STDERR_FILENO);
			ft_putstr_fd(*node.cmd_arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			cenv.exit_status = 255;
			goto exitend; // interdit
		}
		(node.cmd_arg)++;
		if (*node.cmd_arg)
		{
			ft_putstr_fd("msh: exit: too many arguments\n", STDERR_FILENO);
			cenv.exit_status = 1;
			return ;
		}
	}
exitend:
	if (fd[0] > 0)
		close(fd[0]);
	if (fd[1] > 0)
		close(fd[0]);
	msh_exit();
}