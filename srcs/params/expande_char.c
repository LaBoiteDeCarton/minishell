#include "minishell.h"

int		expanded_char_size(char *str)
{
	int		expanded_size;
	int		dquote_open;
	int		squote_open;

	expanded_size = 0;
	dquote_open = 0;
	squote_open = 0;
	while (*str)
	{
		if (*str == '\"' && !squote_open)
			dquote_open = (dquote_open + 1) % 2;
		else if (*str == '\'' && !dquote_open)
			squote_open = (squote_open + 1) % 2;
		else if (*str == '$' && !squote_open)
		{
			expanded_size += ft_strlen(get_value(str + 1));
			str += var_name_size(str + 1);
		}
		else
			expanded_size++;
		str++;
	}
	printf("EXPAND SIZE = %d\n", expanded_size);
	return (expanded_size + 1);
}

char	*expande_char(char *str)
{
	char	*expanded_char;
	int		i;
	int		dquote_open;
	int		squote_open;

	dquote_open = 0;
	squote_open = 0;
	expanded_char = (char *)malloc(sizeof(char) * expanded_char_size(str));
	if (!expanded_char)
	{
		handle_errors("expande failed: ");
		return (NULL);
	}
	i = 0;
	while (*str)
	{
		if (*str == '\"' && !squote_open)
			dquote_open = (dquote_open + 1) % 2;
		else if (*str == '\'' && !dquote_open)
			squote_open = (squote_open + 1) % 2;
		else if (*str == '$' && !squote_open)
		{
			str++;
			ft_memcpy(expanded_char + i, get_value(str), ft_strlen(get_value(str)));
			i += ft_strlen(get_value(str));
			str += var_name_size(str) - 1;
		}
		else
		{
			expanded_char[i] = *str;
			i++;
		}
		str++;
	}
	expanded_char[i] = '\0';
	return (expanded_char);
}