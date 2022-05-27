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

static int	new_char_size(char *str)
{
	size_t	n_c_size;
	int		squote;
	int		dquote;

	n_c_size = 0;
	squote = 0;
	dquote = 0;
	while (*str)
	{
		if (!dquote && !squote && *str == ' ')
			break ;
		if (*str == '\"' && !squote)
			dquote = (dquote + 1) % 2;
		if (*str == '\'' && !dquote)
			squote = (squote + 1) % 2;
		n_c_size++;
		str++;
	}
	return (n_c_size);
}

static t_list	*lst_expande_char(char	*str)
{
	t_list	*lst_res;
	char	*expanded_char;
	int		n_c_size;
	int		i;

	lst_res = NULL;
	expanded_char = first_expand(str);
	i = 0;
	while (expanded_char[i])
	{
		if (expanded_char[i] == ' ')
			i++;
		else
		{
			n_c_size = new_char_size(expanded_char + i);
			ft_lstadd_back(&lst_res,
				ft_lstnew(ft_substr(expanded_char, i, n_c_size)));
			i += n_c_size;
		}
	}
	free(expanded_char);
	return (lst_res);
}

static char	**create_chartab_from_lst(t_list *lst)
{
	char	**chartab;
	int		i;

	chartab = (char **)malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!chartab)
		return (handle_expande_errors("expande failed"));
	i = 0;
	while (lst)
	{
		chartab[i] = (char *)malloc(sizeof(char)
				* (ft_strlen((char *)lst->content) + 1));
		if (!chartab[i])
		{
			handle_errors("expande failed");
			break ;
		}
		ft_copy_without_quotes(chartab[i], (char *)lst->content);
		lst = lst->next;
		i++;
	}
	chartab[i] = NULL;
	return (chartab);
}

static t_list	*expande_wildcard(t_list *lst_excmd)
{
	t_list	*lst_wildcarded;
	t_list	*temp;

	lst_wildcarded = NULL;
	while (lst_excmd)
	{
		if (ft_strrchr((char *)lst_excmd->content, '*'))
		{
			temp = get_wildacred_lst((char *)lst_excmd->content, NULL);
			if (temp)
				ft_lstadd_back(&lst_wildcarded, temp);
			else
				ft_lstadd_back(&lst_wildcarded,
					ft_lstnew(ft_strdup((char *)lst_excmd->content)));
		}
		else
			ft_lstadd_back(&lst_wildcarded,
				ft_lstnew(ft_strdup((char *)lst_excmd->content)));
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
	lst_wildcard = expande_wildcard(lst_excmd);
	ft_lstclear(&lst_excmd, &free);
	node->cmd_arg = create_chartab_from_lst(lst_wildcard);
	ft_lstclear(&lst_wildcard, &free);
	if (node->cmd_arg)
		node->cmd_name = node->cmd_arg[0];
	else
		node->cmd_name = NULL;
}
