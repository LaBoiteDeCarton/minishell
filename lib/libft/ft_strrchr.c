#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*lastocc;

	lastocc = 0;
	while (*s)
	{
		if (*s == (char)c)
			lastocc = (char *)s;
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (lastocc);
}