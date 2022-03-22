#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

char	*ft_strtablast(char **strtab)
{
	if (!strtab)
		return (NULL);
	while (*strtab && *(strtab + 1))
		strtab++;
	return (ft_strdup(*strtab));
}

char	*join_prompt(char *user, char *directory)
{
	char	*join;
	int		size;

	size = ft_strlen(user) + ft_strlen(directory) + ft_strlen(MSH_VERSION) + 6;
	join = (char *)malloc(sizeof(char) * size);
	size = ft_strlcpy(join, MSH_VERSION, ft_strlen(MSH_VERSION) + 1);
	join[size++] = ' ';
	size += ft_strlcpy(join + size, user, ft_strlen(user) + 1);
	join[size++] = ' ';
	size += ft_strlcpy(join + size, directory, ft_strlen(directory) + 1);
	ft_strlcpy(join + size,
		" >", 3);
	free(directory);
	return (join);
}

char	*get_prompt()
{
	char	*user;
	char	*pwd;
	char	**pwd_split;
	char	*directory;

	user = getenv("USER");
	pwd = getenv("PWD");
	pwd_split = ft_split(pwd, '/');
	directory = ft_strtablast(pwd_split);
	free(pwd_split);
	return (join_prompt(user, directory));
}

char	*ft_readline()
{
	if (isatty(STDIN_FILENO))
		return (readline(get_prompt()));
	return (NULL);
}

void	free_content(void *content)
{
	if (((t_lxr *)content)->content)
		free(((t_lxr *)content)->content);
	free((t_lxr *)content);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	cenv = env;
	char	*commande;
	t_list	*lst;

	commande = ft_readline();
	lst = create_lexer(commande);
	printf_lexer(lst);
	ft_lstclear(&lst, &free_content);
	return (0);
}
