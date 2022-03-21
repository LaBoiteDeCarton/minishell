#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*new_s;
	int		i;

	if (!s)
		return (NULL);
	new_s = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!new_s)
		return (NULL);
	else
	{
		i = 0;
		while (s[i])
		{
			new_s[i] = f(i, s[i]);
			i++;
		}
		new_s[i] = 0;
	}
	return (new_s);
}