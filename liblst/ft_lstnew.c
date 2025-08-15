/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:17:37 by llenotre          #+#    #+#             */
/*   Updated: 2018/12/11 18:25:57 by llenotre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"
#include <string.h>

list_t	*ft_lstnew(void const *content, size_t content_size)
{
	list_t *list;

	if (!(list = (list_t*)malloc(sizeof(list_t))))
		return (NULL);
	if (content && content_size > 0)
	{
		if (!(list->content = malloc(content_size)))
		{
			free((void*)list);
			return (NULL);
		}
		memcpy(list->content, content, content_size);
		list->content_size = content_size;
	}
	else
	{
		list->content = NULL;
		list->content_size = 0;
	}
	list->next = NULL;
	return (list);
}
