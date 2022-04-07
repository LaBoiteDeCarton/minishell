#include "minishell.h"

void	free_chartab(char **chartab)
{
	int i;

	i = 0;
	while (chartab[i])
		free(chartab[i++]);
	free(chartab);	
}

static void	clear_var(void *var)
{
	if (((t_var *)var)->name)
		free(((t_var *)var)->name);
	if (((t_var *)var)->value)
		free(((t_var *)var)->value);
}

void	free_var(t_list *var)
{
	ft_lstclear(&var, &clear_var);
}