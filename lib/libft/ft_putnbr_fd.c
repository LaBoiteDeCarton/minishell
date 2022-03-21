#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n >= 0 && n <= 9)
		ft_putchar_fd(n + '0', fd);
	else if (n >= -9 && n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putchar_fd(-n + '0', fd);
	}
	else
	{
		ft_putnbr_fd(n / 10, fd);
		n = n % 10;
		if (n < 0)
			n = -n;
		ft_putchar_fd(n + '0', fd);
	}
}