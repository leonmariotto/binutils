/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_del.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llenotre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 18:19:02 by llenotre          #+#    #+#             */
/*   Updated: 2019/06/08 01:52:18 by llenotre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"

void	default_del(void *content, size_t content_size)
{
	free((void *)content);
	(void)content_size;
}
