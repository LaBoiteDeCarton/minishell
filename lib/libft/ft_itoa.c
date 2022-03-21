#include "libft.h"

static int	itoasize(int n)
{
	int	size;

	if (n < 0)
		size = 2;
	else
		size = 1;
	while (n / 10)
	{
		n /= 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	*a;
	int		nsize;
	int		signe;

	nsize = itoasize(n);
	signe = 1;
	if (n < 0)
		signe = -1;
	a = malloc(sizeof(char) * (nsize + 1));
	if (!a)
		return (NULL);
	a[nsize--] = '\0';
	if (n == 0)
		a[0] = '0';
	else
	{
		if (signe == -1)
			a[0] = '-';
		while (n)
		{
			a[nsize--] = (n % 10) * signe + '0';
			n /= 10;
		}
	}
	return (a);
}