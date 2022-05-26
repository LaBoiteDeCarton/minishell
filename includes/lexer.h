/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:29:15 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:29:16 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"

typedef struct s_lxr	t_lxr;

typedef enum s_lxr_type
{
	word,
	scope_open,
	scope_close,
	read_in,
	open_out,
	append,
	heredoc,
	sep_and,
	sep_or,
	sep_pipe,
	stop,
}	t_lxr_type;

struct s_lxr
{
	t_lxr_type	type;
	char		*content;
};

/*
	Function to create the lexer (token, char) list from the user command
*/
t_list		*create_lexer(char *str);

/*
	checking the validity of the user command
*/
int			lexer_is_valide(t_list	*lst);

/*
	useful function for lexer
*/
t_lxr_type	get_type(t_list *lexer);
int			is_token(t_list *lst);
int			is_redirection(t_list *lst);

/*
	lexer deep free function
*/
void		free_lexer(void *content);

#endif
