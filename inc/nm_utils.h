#ifndef NM_UTILS_H
# define NM_UTILS_H

#include "liblst.h"

int		err_return(char *msg);
void		*err_return_null(char *msg);
void		print_addr(uint64_t addr, int flag, int size);
void		print_info_lg(char info, int global);

#endif
