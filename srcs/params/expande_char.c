#include "minishell.h"

int		expanded_char_size(char *str)
{
	int		expanded_size;

	expanded_size = 0;
	while (*str)
	{
		if (*str == '$')
		{
			expanded_size += ft_strlen(get_value(str + 1));
			str += var_name_size(str + 1) + 1;
		}
		else
		{
			str++;
			expanded_size++;
		}
	}
	return (expanded_size + 1);
}

char	*expande_char(char *str)
{
	char	*expanded_char;
	int		i;

	expanded_char = (char *)malloc(sizeof(char) * expanded_char_size(str));
	if (!expanded_char)
	{
		handle_errors("expande failed: ");
		return (NULL);
	}
	i = 0;
	while (*str)
	{
		if (*str == '$')
		{
			str++;
			ft_memcpy(expanded_char + i, get_value(str), ft_strlen(get_value(str)));
			i += ft_strlen(get_value(str));
			str += var_name_size(str);
		}
		else
		{
			expanded_char[i] = *str;
			i++;
			str++;
		}
	}
	expanded_char[i] = '\0';
	return (expanded_char);
}