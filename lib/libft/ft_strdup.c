#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		len;
	char	*s_cpy;

	len = ft_strlen(s1);
	s_cpy = malloc(sizeof(char) * (len + 1));
	if (!s_cpy)
	{
		errno = ENOMEM;
		return (NULL);
	}
	ft_strlcpy(s_cpy, s1, len + 1);
	return (s_cpy);
}