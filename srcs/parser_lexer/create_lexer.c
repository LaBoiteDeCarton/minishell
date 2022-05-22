#include "lexer.h"
#include "libft.h"
#include <stdio.h>

void	free_content(void *content)
{
	if (((t_lxr *)content)->content)
		free(((t_lxr *)content)->content);
	free((t_lxr *)content);
}

void	printf_lexer(t_list	*lst)
{
	while (lst)
	{
		if (((t_lxr *)lst->content)->type == scope_open)
			printf("scope_open\n");
		else if (((t_lxr *)lst->content)->type == scope_close)
			printf("scope_close\n");
		else if (((t_lxr *)lst->content)->type == read_in)
			printf("read_in\n");
		else if (((t_lxr *)lst->content)->type == open_out)
			printf("open_out\n");
		else if (((t_lxr *)lst->content)->type == append)
			printf("append\n");
		else if (((t_lxr *)lst->content)->type == sep_and)
			printf("sep_and\n");
		else if (((t_lxr *)lst->content)->type == sep_or)
			printf("sep_or\n");
		else if (((t_lxr *)lst->content)->type == sep_pipe)
			printf("sep_pipe\n");
		else if (((t_lxr *)lst->content)->type == heredoc)
			printf("heredoc\n");
		else if(((t_lxr *)lst->content)->type == word)
		{
			printf("word :");
			printf("%s\n", ((t_lxr *)lst->content)->content);
		}
		lst = lst->next;
	}
}

t_lxr_type	lexer_type(char *str)
{
	if (*str == '(')
		return (scope_open);
	else if (*str == ')')
		return (scope_close);
	else if (*str == '\"')
		return (word);
	else if (*str == '\'')
		return (word);
	else if (!ft_strncmp(str, "&&", 2))
		return (sep_and);
	else if (!ft_strncmp(str, "||", 2))
		return (sep_or);
	else if (!ft_strncmp(str, "|", 1))
		return (sep_pipe);
	else if (!ft_strncmp(str, "<<", 2))
		return (heredoc);
	else if (!ft_strncmp(str, ">>", 2))
		return (append);
	else if (!ft_strncmp(str, ">", 1))
		return (open_out);
	else if (!ft_strncmp(str, "<", 1))
		return (read_in);
	else
		return (word);
}

char	*fil_content_from_str(char *str)
{
	int		i;
	char	*res;
	
	i = 0;
	while (str[i] && str[i] != ' ' && lexer_type(str + i) == word)
	{
		if(str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
		}
		if(str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
		}
		if (str[i])
			i++;
	}
	res = (char *)malloc(sizeof(char) * (i + 1));
	ft_strlcpy(res, str, i + 1);
	return (res);
}

void	scroll_str(char **str, t_lxr_type type)
{
	if (type == word)
	{
		while (**str && **str != ' ' && lexer_type(*str) == word)
		{
			if (**str == '\"')
			{
				(*str)++;
				while (**str && **str != '\"')
					(*str)++;
			}
			if (**str == '\'')
			{
				(*str)++;
				while (**str && **str != '\'')
					(*str)++;
			}
			if (**str)
				(*str)++;
		}
	}
	else if (type == heredoc || type == sep_and || type == sep_or || type == append)
		(*str) += 2;
	else
		(*str)++;
}

t_list	*create_lexer(char *str)
{
	t_lxr	*lxr;
	t_list	*res;

	while (*str == ' ' || *str == '\r' || *str == '\n' // attention le \n, a checker
		|| *str == '\f' || *str == '\v' || *str == '\t')
		str++;
	if (!*str)
		return (NULL);
	lxr = (t_lxr *)malloc(sizeof(t_lxr));
	lxr->type = lexer_type(str);
	if (lxr->type == word)
		lxr->content = fil_content_from_str(str);
	else
		lxr->content = NULL;
	scroll_str(&str, lxr->type);
	res = ft_lstnew(lxr);
	ft_lstadd_back(&res, create_lexer(str));
	return (res);
}

/*
	should_be :
		0 - whatever
		1 - a word
		2 - not a token
		not a scope etc..
*/

int		what_is_next(t_lxr_type type)
{
	if (type == word)
		return (0);
	if (type == read_in || type == open_out || type == append | type == heredoc)
		return (1);
	if (type == sep_and || type == sep_or || type == sep_pipe)
		return (0);
	return (0);
}

static int	is_token(t_lxr_type type)
{
	return (type == sep_and || type == sep_or || type == sep_pipe);
}

int		lexer_error(char *err)
{
	ft_putstr_fd("msh: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(err, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (0);
}

char	*get_str_lxr(t_lxr_type type)
{
	if (type == scope_open)
		return ("(");
	if (type == scope_close)
		return (")");
	if (type == read_in)
		return ("<");
	if (type == open_out)
		return (">");
	if (type == append)
		return (">>");
	if (type == heredoc)
		return ("<<");
	if (type == sep_and)
		return ("&&");
	if (type == sep_or)
		return ("||");
	if (type == sep_pipe)
		return ("|");
	return ("newline");
}

int		check_quote_parity(char *str)
{
	int	dq;
	int	sq;

	dq = 0;
	sq = 0;
	while (*str)
	{
		if (*str == '\"' && !sq)
			dq = (dq + 1) % 2;
		if (*str == '\'' && !dq)
			sq = (sq + 1) % 2;
		str++;
	}
	return (sq || dq);
}

int		lexer_is_valide(t_list	*lst)
{
	(void)lst;
	t_list *ptr;
	//exit_status == 258 si KO
	ptr = lst;
	int	should_be;
	int	scope_count;

	should_be = 0;
	scope_count = 0;
	while (ptr)
	{
		if (((t_lxr *)ptr->content)->type == scope_open)
			scope_count++;
		else if (((t_lxr *)ptr->content)->type == scope_close && !scope_count)
			return (lexer_error(")"));
		else if (((t_lxr *)ptr->content)->type == scope_close)
			scope_count--;
		else if (should_be == 1 && ((t_lxr *)ptr->content)->type != word)
			return (lexer_error(get_str_lxr(((t_lxr *)ptr->content)->type)));
		else if (should_be == 2 && is_token(((t_lxr *)ptr->content)->type))
			return (0);
		if (((t_lxr *)ptr->content)->type == word
			&& check_quote_parity(((t_lxr *)ptr->content)->content))
			return (lexer_error("\"' or `'"));
		should_be = what_is_next(((t_lxr *)ptr->content)->type);
		ptr = ptr->next;
	}
	if (scope_count != 0 || should_be)
		return (lexer_error("newline"));
	return (1);
}
