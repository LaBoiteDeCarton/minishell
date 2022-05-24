/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:11:42 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 15:11:43 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>

/*
	si dans expand char, le char a un espace alors ambigious redirect, 
	si path == NULL ambigious redirect aussi
*/

static int	set_stdoutappend(char *path, int *fd)
{
	int		file_fd;
	char	*expanded_path;

	expanded_path = expande_char(path);
	if (!expanded_path || ft_strchr(expanded_path, ' '))
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": ambigious redirect", STDERR_FILENO);
		return (0);
	}
	file_fd = open(expanded_path, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (file_fd == -1)
	{
		handle_errors(expanded_path);
		free(expanded_path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'>>': ");
	*fd = file_fd;
	free(expanded_path);
	return (1);
}

static int	set_stdout(char *path, int *fd)
{
	int		file_fd;
	char	*expanded_path;

	expanded_path = expande_char(path);
	if (!expanded_path || ft_strchr(expanded_path, ' '))
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": ambigious redirect", STDERR_FILENO);
		return (0);
	}
	file_fd = open(expanded_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file_fd == -1)
	{
		handle_errors(expanded_path);
		free(expanded_path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'>'': ");
	*fd = file_fd;
	free(expanded_path);
	return (1);
}

static int	set_stdin(char *path, int *fd)
{
	int		file_fd;
	char	*expanded_path;

	expanded_path = expande_char(path);
	if (!expanded_path || ft_strchr(expanded_path, ' '))
	{
		ft_putstr_fd("msh: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": ambigious redirect", STDERR_FILENO);
		return (0);
	}
	file_fd = open(expanded_path, O_RDONLY);
	if (file_fd == -1)
	{
		handle_errors(expanded_path);
		free(expanded_path);
		return (0);
	}
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'<': ");
	*fd = file_fd;
	free(expanded_path);
	return (1);
}

static int	set_stdinheredoc(int heredoc_fd, int *fd)
{
	if (heredoc_fd == -1)
		return (0);
	if (*fd >= 0 && close(*fd) == -1)
		handle_errors("'<<': ");
	*fd = heredoc_fd;
	return (1);
}

int	redirect(t_redirect redirection, int *fd)
{
	if (redirection.red_type == red_in)
		return (set_stdin(redirection.pathfile, fd));
	else if (redirection.red_type == red_out)
		return (set_stdout(redirection.pathfile, fd + 1));
	else if (redirection.red_type == red_append)
		return (set_stdoutappend(redirection.pathfile, fd + 1));
	else if (redirection.red_type == red_heredoc)
		return (set_stdinheredoc(redirection.fd, fd));
	return (0);
}
