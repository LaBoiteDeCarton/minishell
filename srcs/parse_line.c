#include "ast.h"

/*
scroll to token ? (celui aui est en dehors des guillaumets, en dehors des quotes, et le premier)
recursiv sur gauche puis droit en remplacant les chars du token en /0?
si pas de token ...

*/

int		scroll_until_char(char **str, char c)
{
	int n_scrolled = 0;

	if (!str || !*str)
		return (n_scrolled);
	while (**str && **str != c)
	{
		*str++;
		n_scrolled++;
	}
	return (n_scrolled);
}

int	next_token(char *str)
{
	int	pos;

	if (!str)
		return (-1);
	pos = 0;
	while (*str)
	{
		if (*str == '"')
			pos += scroll_until_char(&str, '"');
		if (*str == '(')
			pos += scroll_until_char(&str, ')'); //ca ne vas pas...
	}
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

t_ast *parse_instruction(char *str)
{
	t_ast *ast;
	(void)str;

	ast = (t_ast *)malloc(sizeof(t_ast));
	ast->token = token_instruction;
	return (ast);
}

void	add_right(t_ast *ast, char *str)
{
	t_ast	*next;

	next = parse_line(str); //si next est vide?
	if (next->token == ast->token)
	{
		if (next->token = token_and)
			ft_lstadd_back(&(((t_and *)ast->content)->content), ((t_and *)next->content)->content);
		else if (next->token = token_or)
			ft_lstadd_back(&(((t_or *)ast->content)->content), ((t_or *)next->content)->content);
		else if (next->token = token_pipe)
			ft_lstadd_back(&(((t_pipe *)ast->content)->content), ((t_pipe *)next->content)->content);
		free(next); //et non ft_lstclear car on garde le contenue
	}
	else
	{
		if (ast->token = token_and)
			ft_lstadd_back(&(((t_and *)ast->content)->content), ft_lstnew(next));
		else if (ast->token = token_or)
			ft_lstadd_back(&(((t_or *)ast->content)->content), ft_lstnew(next));
		else if (ast->token = token_pipe)
			ft_lstadd_back(&(((t_pipe *)ast->content)->content), ft_lstnew(next));
	}
}

t_ast *parse_line(char *str)
{
	char	*trimstr;
	int		next_token_pos;
	t_ast	*ast;

	trimstr = ft_strtrim(str, " \r\n\f\v\t");
	free(str);
	next_token_pos = next_token(trimstr);
	if (next_token_pos == -1)
		return (parse_instruction(trimstr));
	else
	{
		ast = (t_ast *)malloc(sizeof(t_ast));
		ast->token = get_token_type(trimstr, next_token_pos);
		trimstr[next_token_pos] = '\0';
		ast->content = parse_line(trimstr);
		if (ast->token == token_and || ast->token == token_or)
			next_token_pos += 2;
		add_right(ast, trimstr + next_token_pos);
		free(trimstr);
	}
	return (ast);
}


t_list *create_lst_line(char *str)
{
	char	*trimstr;

	trimstr = ft_strtrim(str, " \r\n\f\v\t");
	free(str); // check point
	
	free(trimstr);
}