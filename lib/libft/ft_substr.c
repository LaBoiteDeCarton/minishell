#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_s;
	size_t	ssize;

	if (!s)
		return (NULL);
	ssize = ft_strlen(s);
	if (start > ssize)
		len = 0;
	if (ssize - start < len)
		len = ssize - start;
	sub_s = malloc(sizeof(char) * (len + 1));
	if (!sub_s)
		sub_s = NULL;
	else
	{
		sub_s[len] = 0;
		while (len-- > 0)
			sub_s[len] = s[start + len];
	}
	return (sub_s);
}