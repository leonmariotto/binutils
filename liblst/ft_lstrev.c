/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 01:50:46 by llenotre          #+#    #+#             */
/*   Updated: 2019/06/08 01:50:47 by llenotre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"

void		ft_lstrev(list_t **lst)
{
	list_t *l;

	if (!lst)
		return ;
	l = NULL;
	while (*lst)
	{
		ft_lstadd(&l, *lst);
		*lst = (*lst)->next;
	}
	*lst = l;
}
