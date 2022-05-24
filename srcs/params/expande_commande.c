/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expande_commande.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:11:40 by dmercadi          #+#    #+#             */
/*   Updated: 2022/05/24 16:11:41 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"

static int	itoasize(int n)
{
	int	size;

	if (n < 0)
		size = 2;
	else
		size = 1;
	while (n / 10)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static int	fst_expanded_c_size(char *str)
{
	int		expanded_size;
	int		dquote_open;
	int		squote_open;

	expanded_size = 0;
	dquote_open = 0;
	squote_open = 0;
	while (*str)
	{
		if (*str == '\"' && !squote_open)
			dquote_open = (dquote_open + 1) % 2;
		if (*str == '\'' && !dquote_open)
			squote_open = (squote_open + 1) % 2;
		if (!squote_open && !ft_strncmp(str, "$?", 2))
		{
			expanded_size += itoasize(g_cenv.exit_status);
			str++;
		}
		else if (*str == '$' && !squote_open)
		{
			expanded_size += ft_strlen(get_value(str + 1));
			str += var_name_size(str + 1);
		}
		else
			expanded_size++;
		str++;
	}
	return (expanded_size + 1);
}

char	*first_expand(char *str)
{
	char	*expanded_char;
	int		i;
	int		dquote_open;
	int		squote_open;
	char	*itoachar;

	dquote_open = 0;
	squote_open = 0;
	expanded_char = (char *)malloc(sizeof(char) * fst_expanded_c_size(str));
	if (!expanded_char)
	{
		handle_errors("expande failed: ");
		return (NULL);
	}
	i = 0;
	while (*str)
	{
		if (*str == '\"' && !squote_open)
			dquote_open = (dquote_open + 1) % 2;
		if (*str == '\'' && !dquote_open)
			squote_open = (squote_open + 1) % 2;
		if (!squote_open && !ft_strncmp(str, "$?", 2))
		{
			str++;
			itoachar = get_value(str);
			ft_memcpy(expanded_char + i, itoachar, ft_strlen(itoachar));
			i += ft_strlen(itoachar);
			free(itoachar);
		}
		else if (*str == '$' && !squote_open)
		{
			str++;
			ft_memcpy(expanded_char + i, get_value(str), ft_strlen(get_value(str)));
			i += ft_strlen(get_value(str));
			str += var_name_size(str) - 1;
		}
		else
		{
			expanded_char[i] = *str;
			i++;
		}
		str++;
	}
	expanded_char[i] = '\0';
	return (expanded_char);
}

void	ft_copy_without_quotes(char *dst, char *src, size_t size)
{
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	dquote = 0;
	squote = 0;
	while (size)
	{
		if (*src == '\"' && !squote)
			dquote = (dquote + 1) % 2;
		else if (*src == '\'' && !dquote)
			squote = (squote + 1) % 2;
		else
		{
			dst[i] = *src;
			i++;
		}
		size--;
		src++;
	}
	dst[i] = '\0';
}

t_list	*lst_expande_char(char	*str)
{
	t_list	*lst_res;
	char	*expanded_char;
	char	*new_char;
	size_t	n_c_size;
	int		i;
	int		squote;
	int		dquote;

	lst_res = NULL;
	expanded_char = first_expand(str);
	i = 0;
	while (expanded_char[i])
	{
		if (expanded_char[i] == ' ')
			i++;
		else
		{
			n_c_size = 0;
			squote = 0;
			dquote = 0;
			while (expanded_char[i + n_c_size])
			{
				if (!dquote && !squote && expanded_char[i + n_c_size] == ' ')
					break ;
				if (expanded_char[i + n_c_size] == '\"' && !squote)
					dquote = (dquote + 1) % 2;
				if (expanded_char[i + n_c_size] == '\'' && !dquote)
					squote = (squote + 1) % 2;
				n_c_size++;
			}
			new_char = (char *)malloc(sizeof(char) * n_c_size + 1);
			if (!new_char)
			{
				handle_errors("expande error");
				break ;
			}
			ft_strlcpy(new_char, expanded_char + i, n_c_size + 1);
			ft_lstadd_back(&lst_res, ft_lstnew(new_char));
			i += n_c_size;
		}
	}
	free(expanded_char);
	return (lst_res);
}

char	**create_chartab_from_lst(t_list *lst)
{
	char	**chartab;
	int		i;

	chartab = (char **)malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!chartab)
	{
		handle_errors("malloc");
		return (NULL);
	}
	i = 0;
	while (lst)
	{
		chartab[i] = (char *)malloc(sizeof(char) * ft_strlen((char *)lst->content));
		if (!chartab[i])
		{
			handle_errors("malloc");
			break ;
		}
		ft_copy_without_quotes(chartab[i], (char *)lst->content, ft_strlen((char *)lst->content));
		lst = lst->next;
		i++;
	}
	chartab[i] = NULL;
	return (chartab);
}

t_list	*expande_wildcard(t_list *lst_excmd)
{
	t_list	*lst_wildcarded;

	lst_wildcarded = NULL;
	while (lst_excmd)
	{
		if (ft_strrchr((char *)lst_excmd->content, '*'))
			ft_lstadd_back(&lst_wildcarded, get_dir_lst((char *)lst_excmd->content));
		else
			ft_lstadd_back(&lst_wildcarded, ft_lstnew(ft_strdup((char *)lst_excmd->content)));
		lst_excmd = lst_excmd->next;
	}
	return (lst_wildcarded);
}

void	expande_commande(t_cmd *node)
{
	int		i;
	t_list	*lst_excmd;
	t_list	*lst_wildcard;

	lst_excmd = NULL;
	i = 0;
	while (node->cmd_arg[i])
	{
		ft_lstadd_back(&lst_excmd, lst_expande_char(node->cmd_arg[i]));
		i++;
	}
	free_chartab(node->cmd_arg);
	lst_wildcard = expande_wildcard(lst_excmd); //free
	node->cmd_arg = create_chartab_from_lst(lst_wildcard);
	ft_lstclear(&lst_excmd, &nothing);
	if (node->cmd_arg)
		node->cmd_name = node->cmd_arg[0];
	else
		node->cmd_name = NULL;
}
