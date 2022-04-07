#include "ast.h"
#include "minishell.h"

// int		var_size(char **str)
// {
// 	int	var_name_size;

// 	(*str)++;
// 	var_name_size = 0;
// 	while ((*str)[var_name_size] && ft_isalnum((*str)[var_name_size]))
// 		var_name_size++;
	
// }

// int		expanded_size(char *str)
// {
// 	int	size;

// 	size = 0;
// 	while (*str)
// 	{
// 		if (*str == '$' && ft_isalnum(*(str + 1)))
// 			size += var_size(&str);
// 		else
// 		{
// 			size++;
// 			str++;
// 		}
// 	}
// 	return (size);
// }

// void	char_expand(char **str)
// {
// 	char	*expanded;
// 	int		expand_size;

// 	expand_size = expanded_size(str);
// 	free(*str);
// 	*str = expanded;
// }


