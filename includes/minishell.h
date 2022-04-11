#ifndef MINISHELL_H
# define MINISHELL_H

# define MSH_VERSION "msh-0.1"
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef	struct s_mshenv	t_mshenv;
typedef struct s_var	t_var;

t_mshenv	cenv;

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
};

struct s_var
{
	char	*name;
	char	*value;
};

void	init_signals(void);
void	init_exec_children_signals();
void	init_heredoc_signal();
void	init_exec_father_signal();
char	*ft_readline(char *prompt_msg);
void	ft_system(char *command);
char	*get_prompt();

/*
	handle_error
		Affiche dans la sortie d'erreur le message suivant :
		msh: 'message personalisé': 'message contenu dans ERRNO'
*/
void	handle_errors(char *msg_error);
int		var_name_size(char *name);
char	*expande_char(char *str);

/* 
	environnements functions
*/
void	set_env(char **env);
void	add_to_env(char	*str);

char	*get_value(char *name);



void	free_chartab(char **chartab);
void	free_var(t_list *var);

void	msh_exit();

#endif