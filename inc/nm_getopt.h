
#ifndef NM_GETOPT_H
# define NM_GETOPT_H

# include <stdint.h>
# include <stddef.h>
# include <stdlib.h>
# include <ctype.h>
# include <stdbool.h>
# include "liblst.h"

# define NM_ARCH_M64 1
# define NM_ARCH_M32 2

typedef struct		s_nm_opt {
	bool	config;
	bool	ehdr;
	bool	phdr;
	bool	shdr;
	bool	syms;
	bool	hexdump;
	bool	nm;
	bool	out;
	bool	test;
	char	*outfile;
	t_list	*bins;
}			t_nm_opt;


void	nm_printopt(t_nm_opt *opt);
int	nm_getopt(int ac, char **av, t_nm_opt *opt);

#endif
