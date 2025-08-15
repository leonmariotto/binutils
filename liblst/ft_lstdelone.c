/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:22:21 by llenotre          #+#    #+#             */
/*   Updated: 2018/12/11 18:26:36 by llenotre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"

void	ft_lstdelone(list_t **alst, void (*del)(void*, size_t))
{
	list_t *l;

	if (!alst || !(*alst) || !del)
		return ;
	l = (*alst)->next;
	del((*alst)->content, (*alst)->content_size);
	free((void*)*alst);
	*alst = l;
}
