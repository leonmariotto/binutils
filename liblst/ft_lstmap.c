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

list_t	*ft_lstmap(list_t *lst, list_t *(*f)(list_t *elem))
{
	void	*next;
	list_t	*list;

	if (lst == NULL || f == NULL)
		return (NULL);
	next = lst->next;
	if (!(list = (list_t*)malloc(sizeof(list_t))))
		return (0);
	memcpy(list, f(lst), sizeof(list_t));
	if (next != 0)
		list->next = ft_lstmap(next, f);
	return (list);
}
