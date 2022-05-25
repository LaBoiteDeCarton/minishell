/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 17:00:48 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/25 17:00:49 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if ((!*pattern && *str))
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

/*faire une boucle sur la liste de pattern constitué de ft_split 
du pttern avec '/' */

/*
	on split le pattern en /
	si il existe lelement suivant on check uniquement les paternes qui sont
	des dossiers
		on check dans la liste des dossier le pattern suivant a linterieure
	si cest le dernier ellement on check le pattern de tout
*/

static t_list	*handle_wildcard_error(char *dir)
{
	handle_errors(dir);
	return (NULL);
}

void	join_lst_dir(t_list	*lst, char *left)
{
	char *temp1;
	char *temp2;

	while (lst && left)
	{
		temp1 = ft_strjoin(left, "/");
		temp2 = ft_strjoin(temp1, (char *)lst->content);
		free((char *)lst->content);
		free(temp1);
		lst->content = temp2;
		lst = lst->next;
	}
}

t_list	*get_dir_file_lst(char *pattern, char *dir)
{
	DIR				*dp;
	struct dirent	*dirp;
	t_list			*dir_lst;

	if (!*pattern)
		return (ft_lstnew(ft_strdup(dir)));
	if (dir)
		dp = opendir(dir);
	else
		dp = opendir(g_cenv.tcwd);
	if (!dp)
		return (handle_wildcard_error(dir));
	dir_lst = NULL;
	dirp = readdir(dp);
	while (dirp)
	{
		if (*(dirp->d_name) != '.' && check_patern(pattern, dirp->d_name))
			ft_lstadd_back(&dir_lst, ft_lstnew(ft_strdup(dirp->d_name)));
		dirp = readdir(dp);
	}
	closedir(dp);
	join_lst_dir(dir_lst, dir);
	return (dir_lst);
}

t_list	*get_dir_lst(char *pattern, char *dir)
{
	char			*next;
	t_list			*lst_res;
	DIR				*dp;
	struct dirent	*dirp;

	while (pattern && *pattern && *pattern == '/')
		pattern++;
	next = ft_strchr(pattern, '/');
	if (!next)
		return (get_dir_file_lst(pattern, dir));
	*next = '\0';
	if (dir)
		dp = opendir(dir);
	else
		dp = opendir(g_cenv.tcwd);
	if (!dp)
		return (handle_wildcard_error(dir));
	dirp = readdir(dp);
	lst_res = NULL;
	while (dirp)
	{
		if (dirp->d_type == DT_DIR && *(dirp->d_name) != '.'
			&& check_patern(pattern, dirp->d_name))
 			ft_lstadd_back(&lst_res, get_dir_lst(next + 1, dirp->d_name));
 		dirp = readdir(dp);
	}
	*next = '/';
	join_lst_dir(lst_res, dir);
	return (lst_res);
}