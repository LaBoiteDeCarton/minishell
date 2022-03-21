#include "libft.h"

int ft_strisdigit(char *str)
{
	if (!str)
		return (-1);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
