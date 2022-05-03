#include "ast.h"
#include "minishell.h"
#include "lexer.h"
#include "libft.h"
#include <stdio.h>

void	write_nchar(char c, int n)
{
	while (n-- > 0)
		write(1, &c, 1);
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
}

void	print_instr(t_instruction instr, int prof)
{
	int i;

	if (!instr.cmd)
	{
		write_nchar('|', prof);
		printf("CMD NULL\n");
	}
	if (instr.cmd)
	{
		write_nchar('|', prof);
		printf("CMD : %s\n", instr.cmd->cmd_name);
		i = 0;
		while (instr.cmd->cmd_arg[i])
		{
			write_nchar('|', prof);
			printf("%d-ieme arg : %s\n", i, instr.cmd->cmd_arg[i]);
			i++;
		}
	}
	if (!instr.redirection)
	{
		write_nchar('|', prof);
		printf("NO REDIRECTIONS\n");
	}
	if (instr.redirection)
	{
		write_nchar('|', prof);
		if (((t_redirect *)instr.redirection->content)->pathfile)
			printf("RED PATHFILE : %s\n", ((t_redirect *)instr.redirection->content)->pathfile);
		else
			printf("IS NULL");
	}
}

void	printAST(t_ast *ast, int prof)
{
	t_list *ptr;
	write_nchar('|', prof);
	print_token(ast->token);
	if (ast->token != token_instruction)
	{
		ptr = ast->content;
		while (ptr)
		{
			printAST((t_ast *)ptr->content, prof + 1);
			ptr = ptr->next;
		}
	}
	else
		print_instr(*(t_instruction *)(ast->content->content), prof + 1);
}

t_lxr_type	get_type(t_list *lst)
{
	return (((t_lxr *)lst->content)->type);
}

static int	is_token(t_list *lst)
{
	t_lxr_type	lst_type;

	lst_type = get_type(lst);
	return (lst_type == sep_and
		|| lst_type == sep_or
		|| lst_type == sep_pipe
		|| lst_type == stop);
}

t_list	*next_closed_scope(t_list *lst)
{
	int		open_scope_count;
	t_list	*ptr;

	ptr = lst->next;
	open_scope_count = 1;
	while (ptr && ((t_lxr *)ptr->content)->type != stop)
	{
		if (((t_lxr *)ptr->content)->type == scope_open)
			open_scope_count++;
		if (((t_lxr *)ptr->content)->type == scope_close)
			open_scope_count--;
		if (!open_scope_count)
			return (ptr);
		ptr = ptr->next;
	}
	return (ptr);
}

t_list	*next_token(t_list *lst)
{
	t_list *ptr;

	if (!lst)
		return (NULL);
	if (((t_lxr *)lst->content)->type == scope_open)
	{
		ptr = next_closed_scope(lst);
		if (((t_lxr *)ptr->content)->type != stop
				&& ptr->next
				&& ((t_lxr *)ptr->next->content)->type != stop)
			lst = ptr->next;
	}
	while (lst)
	{
		if (is_token(lst))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	is_redirection(t_list *lst)
{
	t_lxr_type lst_type;

	lst_type = get_type(lst);
	return (lst_type == read_in
		|| lst_type == open_out
		|| lst_type == append
		|| lst_type == heredoc);
}

int	is_text(t_list *lst)
{
	return (get_type(lst) == word);
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

t_list	*add_redirection(t_list *lst)
{
	t_list		*lst_of_redirect;
	t_redirect	*redirection;

	lst_of_redirect = NULL;
	while (lst && !is_token(lst))
	{
		if (is_redirection(lst))
		{
			redirection = NULL;
			redirection = (t_redirect *)malloc(sizeof(t_redirect));
			if (!redirection)
				break ;
			redirection->red_type = get_red(lst);
			redirection->pathfile = NULL;
			lst = lst->next;
			if (is_text(lst))
			{
				redirection->pathfile = ((t_lxr *)lst->content)->content; // ici catch l'erreur si isnot word
				if (redirection->red_type == red_heredoc)
					redirection->fd = get_heredoc(redirection->pathfile);
				ft_lstadd_back(&lst_of_redirect, ft_lstnew(redirection));
			}
			else
				free(redirection);
		}
		lst = lst->next;
	}
	return (lst_of_redirect);
}

int		get_arg_size(t_list *lst)
{
	int 	nb_of_word;

	nb_of_word = 0;
	while (lst && !is_token(lst))
	{
		if (is_text(lst))
			nb_of_word++;
		if (is_redirection(lst))
			lst = lst->next;
		lst = lst->next;
	}
	return (nb_of_word);
}

t_cmd	*add_cmd(t_list *lst)
{
	t_cmd	*cmd;
	int		argsize;
	int		i;

	argsize = get_arg_size(lst);
	if (!argsize)
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_name = NULL;
	cmd->cmd_arg = (char **)malloc(sizeof(char *) * (argsize + 1)); //catch si erreur de malloc
	if (!cmd->cmd_arg)
	{
		free(cmd);
		return (NULL);
	}
	cmd->cmd_arg[argsize] = NULL; //memset tout a NULL please
	i = 0;
	while (lst && !is_token(lst))
	{
		if (is_text(lst) && !cmd->cmd_name)
			cmd->cmd_name = ((t_lxr *)lst->content)->content;
		if (is_text(lst))
			cmd->cmd_arg[i++] = ((t_lxr *)lst->content)->content;
		if (is_redirection(lst))
			lst = lst->next;
		if (!lst || is_token(lst))
		{
			//attention chartab free ici mais memset tout a NULL avant
			free(cmd);
			return (NULL); //ne devrait âs arriver du coup? car handled by lexer_is_valide
		}
		lst = lst->next;
	}
	return (cmd);
}

t_ast	*from_lexer_to_instruction(t_list *lst)
{
	t_ast	*ast;
	t_instruction	*instruction;
	
	if (!lst)
		return (NULL);
	ast = (t_ast *)malloc(sizeof(t_ast)); //checker la reussite du malloc !!!!
	ast->token = token_instruction;
	instruction = (t_instruction *)malloc(sizeof(t_instruction)); //catch si erreur de malloc
	instruction->fd[0] = -1;
	instruction->fd[1] = -1;
	instruction->redirection = add_redirection(lst);
	instruction->cmd = add_cmd(lst);
	ast->content = ft_lstnew(instruction);
	return (ast);
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

/*
 *	Create a AST tree out of lexer 
 * 
 * 	Works recursively :
 * 		1. create a list pointer to the next_token
 * 		2.a If this pointer is NULL, meaning, there is no other token like && || | ;
 * 			we create an "instruction" type of t_ast
 * 		2.b If pointer not NULL, we creating the new t_ast and affecting the token
 * 		3.b Adding to the ast list the asts from the "left" and "right" of the token
 */

t_ast	*from_lexer_to_ast(t_list *lst)
{
	t_list	*ptr;
	t_ast	*ret;

	ptr = next_token(lst);
	if (!ptr || ((t_lxr *)ptr->content)->type == stop)
		return (from_lexer_to_instruction(lst));
	ret = (t_ast *)malloc(sizeof(t_ast)); //catch si erreur de malloc
	ret->token = get_token(ptr);
	((t_lxr *)ptr->content)->type = stop;
	ret->content = NULL;
	join_ast(ret, from_lexer_to_ast(lst));
	return (join_ast(ret, from_lexer_to_ast(ptr->next))); // attention ne gere pas bien les scopes
}