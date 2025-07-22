
#ifndef NM_H
# define NM_H

#include "nm_getopt.h"
#include "nm_bin.h"

int	nm_getopt(int ac, char **av, t_nm_opt *opt);
int	nm(t_nm_opt *opt);

#endif
