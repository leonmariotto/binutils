#ifndef NM_BIN_TRANSFORM_H
# define NM_BIN_TRANSFORM_H

#include "nm_bin.h"

#define NM_CODECAVE_INJECT 0
#define NM_SILVIO_INJECT 1
#define NM_SECTION_INJECT 2

// TODO add sym
typedef struct s_nm_bin_transform {
	uint8_t 	type;
	uint8_t		*code;
	uint64_t	code_len;
} t_nm_bin_transform;

int		nm_bin_transform_section_injection(t_nm_bin *bin, t_nm_bin_transform *transform);
int		nm_bin_transform_silvio_injection(t_nm_bin *bin, t_nm_bin_transform *transform);
int		nm_bin_transform_codecave_injection(t_nm_bin *bin, t_nm_bin_transform *transform);

int		nm_bin_transform(t_nm_bin *bin, t_nm_bin_transform *transform);

#endif /* NM_BIN_TRANSFORM_H */
