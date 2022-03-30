#ifndef MINISHELL_H
# define MINISHELL_H

# define MSH_VERSION "msh-0.1"
# include "libft.h"

typedef	struct s_mshenv	t_mshenv;

t_mshenv	cenv;

/* Minishell Env Structure :
	- Pointeur vers l'env du programme
	- Entier stockant la derniere valeur de sortie de la derniere commande effectuée
	- Liste de char * contenant les variables d'environnement
*/
struct s_mshenv
{
	char **env;
	int	exit_status;
	t_list	*var;
};

void	init_signals(void);
void	ft_system(char *command);
char	*get_prompt();

#endif