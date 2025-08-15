/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_nodup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 15:53:41 by llenotre          #+#    #+#             */
/*   Updated: 2019/05/30 20:57:48 by lmariott         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"
#include <string.h>

list_t	*ft_lstnew_nodup(void *content, size_t content_size)
{
	list_t *list;

	if (!(list = (list_t *)malloc(sizeof(list_t))))
		return (NULL);
	bzero(list, sizeof(list_t));
	if (content && content_size > 0)
	{
		list->content = content;
		list->content_size = content_size;
	}
	return (list);
}
