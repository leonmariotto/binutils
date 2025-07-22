/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:23:12 by llenotre          #+#    #+#             */
/*   Updated: 2018/12/11 18:24:37 by llenotre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"

void	ft_lstadd(t_list **alst, t_list *new)
{
	t_list	*next;

	if (alst == NULL)
		return ;
	if (*alst == NULL)
		*alst = new;
	else
	{
		next = *alst;
		*alst = new;
		new->next = next;
	}
}
