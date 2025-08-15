/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstswap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmariott <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:06:03 by lmariott          #+#    #+#             */
/*   Updated: 2022/06/28 10:34:41 by lmariott         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"

void				ft_lstswap(list_t *l0, list_t **l1, list_t **l2)
{
	list_t 	*tmp1;
	list_t	*save;

	tmp1 = *l1;
	*l1 = *l2;
	save = (*l2)->next;
	*l2 = tmp1;
	(*l1)->next = *l2;
	(*l2)->next = save;
	if (l0)
		l0->next = *l1;
}
