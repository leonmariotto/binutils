#include "elftool.h"
#include "stdio.h"
#include <elf.h>
#include <endian.h>
#include <byteswap.h>
#include <arpa/inet.h>
#include "elftool_dump.h"
#include "elftool_write.h"
#include "elftool_parse.h"
#include <string.h>


#define STBLOCKS_UNIT 512

int	elftool_injection(elftool_opt_t *opt, elftool_t *bin)
{
	int r = 0;

	if (opt->out) {
		// elftool_t_transform transform = {0};
		// transform.code = malloc(333);
		// transform.code_len = 333;
		// transform.type = NM_SECTION_INJECT;
		// elftool_transform(bin, &transform);
		r = elftool_write(bin, opt->outfile);
	}
	return (r);
}

static void	elftool_bin_clean(elftool_t *bin)
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

int	elftool_load(elftool_t *bin, char *bin_path)
{
	struct stat	stats;
	int fd;
	int r = 0;
	
	if ((fd = open(bin->path, O_RDONLY)) <= 0) {
		r = EINVAL;
		fprintf(stderr, "open failed");
	}
	if (r == 0) {
		if (fstat(fd, &stats)) {
			r = EINVAL;
			fprintf(stderr, "fstat failed");
		}
	}
	if (r == 0) {
		if ((stats.st_mode & S_IFMT) != S_IFREG) {
			r = EINVAL;
			fprintf(stderr, "This is not a regular file, abort");
		}
	}
	if (r == 0) {
		bin->length = stats.st_blocks * STBLOCKS_UNIT;
		if (bin->length % 4096) {
			r = EINVAL;
			fprintf(stderr, "bin->length not aligned to pagesize");
		}
	}
	if (r == 0) {
		if (!(bin->mem = mmap(NULL, bin->length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))) {
			r = EINVAL;
			fprintf(stderr, "mmap call failed");
		}
	}
	if (fd >= 0) {
		close(fd);
	}
	return (r);
}

void elftool_clean(elftool_opt_t *opt)
{
	if (opt->bins) {
		ft_lstdel(&opt->bins, default_del);
	}
}

int	elftool(elftool_opt_t *opt)
{
	t_list		*head;
	int r = 0;
	bool printName = false;
	elftool_t	bin = {0};

	if (!opt)
	{
		r = -1;
	}
	else
	{
		head = opt->bins;
		if (!head)
		{
			r = -1;
		}
	}
	if (r == 0)
	{
		if (head->next) {
			printName = true;
		}
		while (r == 0 && head)
		{
			bzero(&bin, sizeof(bin));
			if (printName) {
				printf("\n%s\n", bin.path);
			}
			r = elftool_load(&bin, head->content);
			if (r == 0) {
				r = elftool_parse(&bin);
			}
			if (r == 0) {
				elftool_dump(opt, &bin);
			}
			if (r == 0) {
				elftool_injection(opt, &bin);
			}
			elftool_bin_clean(&bin);
			head = head->next;
		}
	}
	elftool_clean(opt);
	return (r);
}
