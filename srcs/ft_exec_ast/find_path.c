/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:34:04 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/25 16:34:06 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

static char	*find_path_two(char	*f, char *newf, char **paths)
{
	int		i;
	char	*fpath;

	i = 0;
	while (paths && paths[i])
	{
		fpath = ft_strjoin(paths[i], newf);
		if (!fpath)
			handle_errors("Command: ");
		else if (access(fpath, F_OK) == 0)
		{
			free(f);
			free(newf);
			free_chartab(paths);
			return (fpath);
		}
		if (fpath)
			free(fpath);
		i++;
	}
	free_chartab(paths);
	free(newf);
	return (NULL);
}

char	*find_path(char *f)
{
	char	**paths;
	char	*newf;

	if (!ft_strlen(f))
		return (NULL);
	if (ft_strchr(f, '/'))
		return (f);
	newf = ft_strjoin("/", f);
	paths = ft_split(get_value("PATH"), ':');
	return (find_path_two(f, newf, paths));
}
