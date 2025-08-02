/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leon <lmariott@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:34:17 by leon              #+#    #+#             */
/*   Updated: 2022/06/03 14:41:59 by lmariott         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elftool.h"

int main(int ac, char **av) {
  int r;
  elftool_opt_t opt = {0};

  r = elftool_getopt(ac, av, &opt);
  //	nm_printopt(&opt);
  if (r == 0) {
    r = elftool(&opt);
  }
  return (r);
}
