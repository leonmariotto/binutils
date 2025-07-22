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

t_list	*ft_lstnew_nodup(void *content, size_t content_size)
{
	t_list *list;

	if (!(list = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	bzero(list, sizeof(t_list));
	if (content && content_size > 0)
	{
		list->content = content;
		list->content_size = content_size;
	}
	return (list);
}
