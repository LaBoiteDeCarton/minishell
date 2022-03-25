#ifndef LEXER_H
# define LEXER_H

# include "libft.h"

typedef struct s_lxr	t_lxr;

typedef enum s_lxr_type
{
	word,
	scope_open,
	scope_close,
	single_qt,
	double_qt,
	read_in,
	open_out,
	append,
	heredoc,
	sep_and,
	sep_or,
	sep_pipe,
	stop
}	t_lxr_type;

struct s_lxr
{
	t_lxr_type	type;
	char		*content;
};

t_list	*create_lexer(char *str);
void	printf_lexer(t_list	*lst);
void	free_content(void *content);
int		lexer_is_valide(t_list	*lst);

#endif