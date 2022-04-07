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
			(*str)++;
		}
	}
	else if (type == heredoc || type == sep_and || type == sep_or || type == append)
		(*str) += 2;
	else
		(*str)++;
}

// char	*fil_sqt_content_from_str(char *str)
// {
// 	int		i;
// 	char	*res;
	
// 	i = 0;
// 	str++;
// 	while (str[i] && str[i] != '\'')
// 		i++;
// 	res = (char *)malloc(sizeof(char) * (i + 1));
// 	ft_strlcpy(res, str, i + 1);
// 	return (res);
// }

// char	*fil_dqt_content_from_str(char *str)
// {
// 	int		i;
// 	char	*res;
	
// 	i = 0;
// 	str++;
// 	while (str[i] && str[i] != '\"')
// 		i++;
// 	res = (char *)malloc(sizeof(char) * (i + 1));
// 	ft_strlcpy(res, str, i + 1);
// 	return (res);
// }

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

int		lexer_is_valide(t_list	*lst)
{
	(void)lst;
	//exit_status == 258 si KO
	return (1);
}