#ifndef MINISHELL_H
# define MINISHELL_H

# define MSH_VERSION "msh-0.1"

void	init_signals(void);
void	ft_system(char *command);
char	*get_prompt();

#endif