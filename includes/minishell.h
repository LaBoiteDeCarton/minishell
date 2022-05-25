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
	- Entier stockant la derniere valeur de sortie de la derniere commande effectuée
	- Liste de char * contenant les variables d'environnement
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

void	init_signals(void);
void	init_child_sig(void);
void	init_heredoc_signal(void);
void	init_father_sig(void);
char	*ft_readline(char *prompt_msg);
void	ft_system(char *command);

/*
	handle_error
		Affiche dans la sortie d'erreur le message suivant :
		msh: 'message personalisé': 'message contenu dans ERRNO'
*/
void	handle_errors(char *msg_error);

size_t	var_name_size(char *name);
char	*expande_char(char *str);
void	expande_commande(t_cmd *node);
t_list	*get_dir_lst(char *pattern, char *dir);

/* 
	environnements functions
*/
void	set_env(char **env);
void	add_to_env(char	*str);
void	add_param(char *param);
void	add_params(char **chartab);
void	del_param(char *name);
void	del_from_env(char *str);
char	*get_value(char *name);

void	free_chartab(char **chartab);
void	free_var(t_list *var);
void	clear_var(void *var);

void	msh_exit(void);

#endif
