#include "minishell.h"
#include "dirent.h"

int	check_patern(char *pattern, char *str)
{
	while (*pattern && *pattern != '*')
	{
		if (*str != *pattern)
			return (0);
		str++;
		pattern++;
	}
	if ((!pattern && str))
		return (0);
	while (*pattern && *pattern == '*')
		pattern++;
	if (!*pattern)
		return (1);
	while (*str)
	{
		if (check_patern(pattern, str))
			return (1);
		str++;
	}
	return (0);
}

t_list	*get_dir_lst(char	*pattern)
{
	DIR				*dp;
    struct dirent	*dirp;
	t_list			*dir_lst;

	dp = opendir(g_cenv.tcwd);
	if (!dp)
		return (ft_lstnew(pattern));
	dir_lst = NULL;
	dirp = readdir(dp);
	while (dirp)
	{
		if (*(dirp->d_name) != '.' && check_patern(pattern, dirp->d_name))
			ft_lstadd_back(&dir_lst, ft_lstnew(ft_strdup(dirp->d_name)));
		dirp = readdir(dp);
	}
	closedir(dp);
	if (ft_lstsize(dir_lst) == 0)
		return (ft_lstnew(pattern));
	return (dir_lst);
}