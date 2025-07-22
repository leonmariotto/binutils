#include "nm_bin.h"
#include "nm_bin_write.h"
#include "nm_bin_transform.h"
#include "nm_bin_test.h"
#include "nm_utils.h"
#include "stdio.h"
#include <elf.h>
#include <endian.h>
#include <byteswap.h>
#include <arpa/inet.h>

int	nm_bin_parse(t_nm_bin *bin)
{
	int r = 0;

	if (r == 0) {
		r = nm_bin_parse_ehdr(bin);
	}
	if (r == 0) {
		r = nm_bin_parse_phdr(bin);
	}
	if (r == 0) {
		r = nm_bin_parse_shdr(bin);
	}
	if (r == 0) {
		r = nm_bin_parse_syms(bin);
	}
	return (r);
}

void	nm_bin_dump(t_nm_opt *opt, t_nm_bin *bin)
{

	if (opt->ehdr) {
		nm_bin_dump_ehdr(bin);
	}
	if (opt->phdr) {
		nm_bin_dump_phdr(bin);
	}
	if (opt->shdr) {
		nm_bin_dump_shdr(bin);
	}
	if (opt->syms) {
		nm_bin_dump_syms(bin);
	}
	if (opt->nm) {
		nm_bin_dump_nm(bin);
	}
}

int	nm_bin_injection(t_nm_opt *opt, t_nm_bin *bin)
{
	int r = 0;

	if (opt->out) {
		// t_nm_bin_transform transform = {0};
		// transform.code = malloc(333);
		// transform.code_len = 333;
		// transform.type = NM_SECTION_INJECT;
		// nm_bin_transform(bin, &transform);
		r = nm_bin_write(bin, opt->outfile);
	}
	return (r);
}

static void	nm_bin_clean(t_nm_bin *bin)
{
	if (bin->shdr) {
		ft_lstdel(&bin->shdr, default_del);
	}
	if (bin->phdr) {
		ft_lstdel(&bin->phdr, default_del);
	}
	if (bin->syms) {
		ft_lstdel(&bin->syms, default_del);
	}
}

int	nm_bin(t_nm_opt *opt, t_nm_bin *bin)
{
	int r = 0;

	r = nm_bin_parse(bin);
	if (r == 0) {
		nm_bin_dump(opt, bin);
	}
	if (r == 0) {
		nm_bin_injection(opt, bin);
	}
	if (r == 0) {
		if (opt->test) {
			nm_bin_test(bin);
		}
	}
	nm_bin_clean(bin);
	return (r);
}
