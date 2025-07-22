
#include "nm_bin.h"

int		nm_bin_test(t_nm_bin *bin)
{
	while (bin->length > 1)
	{
		bin->length--;
		nm_bin_parse(bin);
		nm_bin_dump_nm(bin);
	}
	return (0);
}
