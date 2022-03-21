#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int	i;

	while (len > 0 && *haystack)
	{
		i = 0;
		while (len - i > 0 && needle[i] && haystack[i] == needle[i])
			i++;
		if (!(needle[i]))
			return ((char *)haystack);
		haystack++;
		len--;
	}
	if (!*needle)
		return ((char *)haystack);
	return (NULL);
}