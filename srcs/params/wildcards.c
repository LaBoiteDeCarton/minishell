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

static int	check_patern(char *pattern, char *str)
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

static char	*join_dir(char *left, char *right)
{
	char	*temp1;
	char	*temp2;

	if (left)
	{
		temp1 = ft_strjoin(left, right);
		temp2 = ft_strjoin(temp1, "/");
		free(temp1);
	}
	else
		temp2 = ft_strjoin(right, "/");
	return (temp2);
}

static t_list	*get_dir_and_file_lst(char *pattern, char *dir)
{
	DIR				*dp;
	struct dirent	*dirp;
	t_list			*dir_lst;

	if (!*pattern)
		return (ft_lstnew(dir));
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
			ft_lstadd_back(&dir_lst, ft_lstnew(ft_strjoin(dir, dirp->d_name)));
		dirp = readdir(dp);
	}
	closedir(dp);
	if (dir)
		free(dir);
	return (dir_lst);
}

static t_list	*get_dir_lst(char *pattern, char *next, char *dir)
{
	DIR				*dp;
	struct dirent	*dirp;
	t_list			*lst_match;

	if (dir)
		dp = opendir(dir);
	else
		dp = opendir(g_cenv.tcwd);
	if (!dp)
		return (handle_wildcard_error(dir));
	lst_match = NULL;
	dirp = readdir(dp);
	while (dirp)
	{
		if (dirp->d_type == DT_DIR && *(dirp->d_name) != '.'
			&& check_patern(pattern, dirp->d_name))
			ft_lstadd_back(&lst_match,
				get_wildacred_lst(next + 1, join_dir(dir, dirp->d_name)));
		dirp = readdir(dp);
	}
	closedir(dp);
	if (dir)
		free(dir);
	return (lst_match);
}

t_list	*get_wildacred_lst(char *pattern, char *dir)
{
	char			*next;
	t_list			*lst_match;

	while (pattern && *pattern && *pattern == '/')
		pattern++;
	next = ft_strchr(pattern, '/');
	if (!next)
		return (get_dir_and_file_lst(pattern, dir));
	*next = '\0';
	lst_match = get_dir_lst(pattern, next, dir);
	*next = '/';
	return (lst_match);
}
