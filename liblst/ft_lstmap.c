/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:24:16 by llenotre          #+#    #+#             */
/*   Updated: 2018/12/11 18:25:37 by llenotre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"
#include <string.h>

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	void	*next;
	t_list	*list;

	if (lst == NULL || f == NULL)
		return (NULL);
	next = lst->next;
	if (!(list = (t_list*)malloc(sizeof(t_list))))
		return (0);
	memcpy(list, f(lst), sizeof(t_list));
	if (next != 0)
		list->next = ft_lstmap(next, f);
	return (list);
}
