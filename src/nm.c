#include "nm_bin.h"

#include <string.h>

#define STBLOCKS_UNIT 512

int	nm_bin_load(t_nm_bin *bin)
{
	struct stat	stats;
	int fd;
	
	if ((fd = open(bin->path, O_RDONLY)) <= 0)
		return (err_return("open failed"));
	if (fstat(fd, &stats)) {
		close(fd);
		return (err_return("fstat failed"));
	}
	if ((stats.st_mode & S_IFMT) != S_IFREG) {
		close(fd);
		return (err_return("This is not a regular file, abort"));
	}
	bin->length = stats.st_blocks * STBLOCKS_UNIT;
	if (bin->length % 4096) {
		close(fd);
		return (err_return("bin->length not aligned to pagesize"));
	}
	if (!(bin->mem = mmap(NULL, bin->length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0))) {
		close(fd);
		return (err_return("mmap call failed"));
	}
	close(fd);
	return (0);
}

void nm_clean(t_nm_opt *opt)
{
	if (opt->bins) {
		ft_lstdel(&opt->bins, default_del);
	}
}

int	nm(t_nm_opt *opt)
{
	t_list		*head;
	int r = 0;
	bool printName = false;
	t_nm_bin	bin = {0};

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
			bin.path = head->content;
			if (printName) {
				write(1, "\n", 1);
				printf("%s", bin.path);
				write(1, ":\n", 2);
			}
			r = nm_bin_load(&bin);
			if (r == 0) {
				r = nm_bin(opt, &bin);
			}
			head = head->next;
		}
	}
	nm_clean(opt);
	return (r);
}
