/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmariott <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 13:05:56 by lmariott          #+#    #+#             */
/*   Updated: 2022/06/28 10:36:40 by lmariott         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "liblst.h"
#include <stdint.h>
#include <stdio.h>
#include <elf.h>
#include <ctype.h>

int		err_return(char *msg)
{
	dprintf(2, "%s\n", msg);
	return (-1);
}

void		*err_return_null(char *msg)
{
	dprintf(2, "%s\n", msg);
	return (NULL);
}

void		print_addr(uint64_t addr, int flag, int size)
{
	int i;

	if (flag)
		dprintf(2, "%0*lx\n", size, addr);
	else
	{
		i = -1;
		while (++i < size)
			printf(" ");
	}
	printf(" ");
}

void		print_info_lg(char info, int global)
{
	if (global)
		info = toupper(info);
	printf("%c ", info);
}
