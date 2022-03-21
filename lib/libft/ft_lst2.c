/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 12:25:35 by dmercadi          #+#    #+#             */
/*   Updated: 2021/03/24 12:34:16 by dmercadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next_temp;

	while (*lst)
	{
		next_temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = next_temp;
	}
	free(*lst);
	*lst = NULL;
	lst = NULL;
}

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (lst)
	{
		f(lst->content);
		ft_lstiter(lst->next, f);
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;

	if (!lst)
		return (NULL);
	new_lst = ft_lstnew(f(lst->content));
	if (!new_lst)
		return (NULL);
	new_lst->next = ft_lstmap(lst->next, f, del);
	if (!new_lst->next && lst->next)
	{
		ft_lstdelone(new_lst, del);
		return (NULL);
	}
	return (new_lst);
}
