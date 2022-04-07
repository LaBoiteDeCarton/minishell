#include "minishell.h"
#include "libft.h"

static char	*ft_strtablast(char **strtab)
{
	if (!strtab)
		return (NULL);
	while (*strtab && *(strtab + 1))
		strtab++;
	return (ft_strdup(*strtab));
}

static char	*join_prompt(char *user, char *directory)
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
	free_chartab(pwd_split);
	return (join_prompt(user, directory));
}