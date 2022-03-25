#include "ast.h"
#include "lexer.h"
#include "libft.h"
#include <stdio.h>

void	write_nchar(char c, int n)
{
	while (n-- > 0)
		write(1, &c, 1);
}

void	print_token(t_token token)
{
	if (token == token_and)
		printf("AND\n");
	if (token == token_or)
		printf("OR\n");
	if (token == token_pipe)
		printf("PIPE\n");
	if (token == token_instruction)
		printf("INSTR\n");
	if (token == token_cmd)
		printf("CMD\n");
	if (token == token_instruction)
		printf("INSTR\n");
}

void	printAST(t_ast *ast, int prof)
{
	write_nchar('|', prof);
	print_token(ast->token);
	if (ast->token != token_instruction)
	{
		while (ast->content)
		{
			printAST((t_ast *)ast->content, prof + 1);
			ast->content = ast->content->next;
		}
	}
}

int	is_token(t_list *lst)
{
	return (((t_lxr *)lst->content)->type == sep_and
		|| ((t_lxr *)lst->content)->type == sep_or
		|| ((t_lxr *)lst->content)->type == sep_pipe
		|| ((t_lxr *)lst->content)->type == stop);
}

t_list	*last_closed_scope(t_list *lst)
{
	t_list	*next;

	if (!lst)
		return (NULL);
	next = last_closed_scope(lst->next);
	if (next)
		return (next);
	if (((t_lxr *)lst->content)->type == scope_close)
		return (lst);
	return (NULL);
}

t_list	*next_token(t_list *lst)
{
	while (lst && !is_token(lst))
	{
		if (((t_lxr *)lst->content)->type == scope_open)
			lst = last_closed_scope(lst)->next;
		else
			lst = lst->next;
	}
	if (!lst || ((t_lxr *)lst->content)->type == stop)
		return (NULL); //spaghetti?
	return (lst);
}

int	is_redirection(t_list *lst)
{
	return (((t_lxr *)lst->content)->type == read_in
		|| ((t_lxr *)lst->content)->type == open_out
		|| ((t_lxr *)lst->content)->type == append
		|| ((t_lxr *)lst->content)->type == heredoc);
}

int	is_text(t_list *lst)
{
	return (((t_lxr *)lst->content)->type == single_qt
		|| ((t_lxr *)lst->content)->type == double_qt
		|| ((t_lxr *)lst->content)->type == word);
}

t_redirect_type get_red(t_list *lst)
{
	if (((t_lxr *)lst->content)->type == read_in)
		return (red_in);
	if (((t_lxr *)lst->content)->type == open_out)
		return (red_out);
	if (((t_lxr *)lst->content)->type == append)
		return (red_append);
	if (((t_lxr *)lst->content)->type == heredoc)
		return (red_heredoc);
	return (red_error);
}

void	add_redirection(t_instruction *instr, t_list **lst)
{
	t_redirect	*redirection;

	redirection = (t_redirect *)malloc(sizeof(t_redirect));
	redirection->red_type = get_red(*lst);
	redirection->pathfile = NULL;
	if ((*lst)->next && is_text((*lst)->next))
	{
		redirection->pathfile = ((t_lxr *)(*lst)->next->content)->content;
		*lst = (*lst)->next;
	}
	*lst = (*lst)->next;
	ft_lstadd_back(&instr->redirection, ft_lstnew(redirection));
}

char	**create_chartab(t_list *lst)
{
	int 	i;
	t_list	*ptr;
	char	**chartab;

	ptr = lst;
	i = 0;
	while (ptr && is_text(ptr))
	{
		ptr = ptr->next;
		i++;
	}
	chartab = (char **)malloc(sizeof(char *) * (i + 1));
	chartab[i] = NULL;
	i = 0;
	while (lst && is_text(lst))
	{
		chartab[i] = ((t_lxr *)lst->content)->content;
		i++;
		lst = lst->next;
	}
	return (chartab);
}

void	add_cmd(t_instruction *instr, t_list **lst)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->cmd_name = ((t_lxr *)(*lst)->content)->content;
	cmd->cmd_arg = NULL;
	cmd->cmd_arg = create_chartab(*lst);
		*lst = (*lst)->next;
	while (*lst && is_text(*lst))
		*lst = (*lst)->next;
	instr->cmd = cmd;
}

t_ast	*from_lexer_to_instruction(t_list *lst)
{
	t_ast	*ast;
	t_instruction	*instruction;
	
	if (!lst)
		return (NULL);
	ast = (t_ast *)malloc(sizeof(t_ast)); //checker la reussite du malloc !!!!
	ast->token = token_instruction;
	instruction = (t_instruction *)malloc(sizeof(t_instruction));
	instruction->cmd = NULL;
	instruction->redirection = NULL;
	while (lst && ((t_lxr *)lst->content)->type != stop)
	{
		if (is_redirection(lst))
			add_redirection(instruction, &lst);
		else if (is_text(lst))
			add_cmd(instruction, &lst);
		else
			lst = lst->next;
	}
	ast->content = ft_lstnew(instruction);
	return (ast);
}

t_token	get_token(t_list *lst)
{
	if (((t_lxr *)lst->content)->type == sep_and)
		return (token_and);
	if (((t_lxr *)lst->content)->type == sep_or)
		return (token_or);
	if (((t_lxr *)lst->content)->type == sep_pipe)
		return (token_pipe);
	return (token_error);
}

t_ast	*join_ast(t_ast *left, t_ast *right)
{
	if (left->token == right->token)
	{
		ft_lstadd_back(&(left->content), right->content);
		free(right); //leaks check
	}
	else
		ft_lstadd_back(&(left->content), ft_lstnew(right));
	return (left);
}

t_ast	*from_lexer_to_ast(t_list *lst)
{
	t_list	*ptr;
	t_ast	*ret;

	ptr = next_token(lst);
	if (!ptr)
		return (from_lexer_to_instruction(lst));
	ret = (t_ast *)malloc(sizeof(t_ast));
	ret->token = get_token(ptr);
	((t_lxr *)ptr->content)->type = stop;
	ret->content = ft_lstnew(from_lexer_to_ast(lst)); //si ft_lstnew echou, leaks... attention
	return (join_ast(ret, from_lexer_to_ast(ptr->next))); // attention ne gere pas bien les scopes
}