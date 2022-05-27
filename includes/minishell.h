/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:29:21 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:29:22 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MSH_VERSION "msh-0.1"
# include "libft.h"
# include "ast.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_mshenv	t_mshenv;
typedef struct s_var	t_var;

t_mshenv				g_cenv;

/* Minishell Env Structure :
	- Pointeur vers l'env du programme
	- Entier contenant la taille de env;
	- Entier stockant la derniere valeur de sortie de la derniere commande
		effectuée
	- Liste de char * contenant les variables d'environnement
	- Chaine de caractere de "The Current Work Directory"
	- Liste d'arbre AST de la commande courante
*/
struct s_mshenv
{
	char			**env;
	int				env_s;
	unsigned int	exit_status;
	t_list			*var;
	char			*tcwd;
	t_list			*ast;
};

struct s_var
{
	char	*name;
	char	*value;
};

/*
	Msh core functions
		ft_readline : like readline but checking tty of standart input
		ft_system : like system(<char *: commande>) but handmade
		msh_exit : like exit(<int: exit status>) but clearing every
			malloc attribut
*/
char	*ft_readline(char *prompt_msg);
void	ft_system(char *command);
void	msh_exit(void);

/*
	Signal function changes the signal event behaviours
*/
void	init_signals(void);
void	init_child_sig(void);
void	init_heredoc_signal(void);
void	init_father_sig(void);

/*
	Error handlers function
		printing in the standart error output the folowing canvas :
			"msh: <char>: <ERRNO translate>"	
*/
void	handle_errors(char *msg_error);
t_list	*handle_wildcard_error(char *dir);
int		lexer_error(char *err);
void	*handle_ast_error(t_ast *ast, t_cmd *cmd, char *str);
void	*handle_expande_errors(char *msg_error);

/*
	Expande function.
	Expand means changing the $VAR_NAME in string by it's value (following
		rules) 
*/
size_t	var_name_size(char *name);
char	*expande_char(char *str);
char	*first_expand(char *str);
int		fst_expanded_c_size(char *str);
void	ft_copy_without_quotes(char *dst, char *src);
void	expande_commande(t_cmd *node);
t_list	*get_wildacred_lst(char *pattern, char *dir);

/* 
	Environnement functions
*/
void	set_env(char **env);
void	add_to_env(char	*str);
void	add_param(char *param);
void	add_params(char **chartab);
void	del_param(char *name);
void	del_from_env(char *str);
char	*get_value(char *name);

/*
	Free functions
		free_chartab make a deep free of a char**
		free_var make a deep free of t_var
		clear_var make a deep free of t_var list
		
*/
void	free_chartab(char **chartab);
void	free_var(t_list *var);
void	clear_var(void *var);

#endif
