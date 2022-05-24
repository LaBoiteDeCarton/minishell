/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:31:42 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 14:31:43 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_readline(char *prompt_msg)
{
	char	*read_buffer;

	read_buffer = NULL;
	if (isatty(STDIN_FILENO))
		read_buffer = readline(prompt_msg);
	else
		get_next_line(STDIN_FILENO, &read_buffer);
	return (read_buffer);
}
