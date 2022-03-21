#include "ast.h"

/*
scroll to token ? (celui aui est en dehors des guillaumets, en dehors des quotes, et le premier)
recursiv sur gauche puis droit en remplacant les chars du token en /0?
si pas de token



*/

int	next_token(char *str)
{
	if (*str == '(')
		;
	
}

t_token	get_token_type(char *str, int n)
{
	str += n;
	if (ft_strncmp(str, "&&", 2))
		return (token_and);
	else if (ft_strncmp(str, "||", 2))
		return (token_or);
	else if (ft_strncmp(str, "|", 2))
		return (token_pipe);
	else
		return (token_error);
}

t_ast *parse_line(char *str)
{
	char	*trimstr;
	int		next_token_pos;
	t_ast	*ast;

	trimstr = ft_strtrim(str, " \r\n\f\v\t");
	free(str);
	next_token_pos = next_token(trimstr);
	if (next_token == -1)
		return (parse_instruction(trimstr));
	else
	{
		ast = (t_ast *)malloc(sizeof(t_ast));
		ast->token = get_token_type(trimstr, next_token_pos);
		ast->content = 
	}
}