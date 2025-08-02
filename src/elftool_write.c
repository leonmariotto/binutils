#include "elftool.h"

// TODO use fseek to replace ptr in file
// woody allow the use of fpusts, fflush, lseek
// or use an allocated space and one write, that's better but we don't know the size
void	elftool_write64(elftool_t *bin, int fd)
{
	if (bin)
	{
		write(fd, bin->ehdr64, 0x40);
		lseek(fd, bin->ehdr64->e_phoff, SEEK_SET);
		for (t_list *head = bin->phdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((phdr64_t*)head->content)->phdr, bin->ehdr64->e_phentsize);
			}
		}
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				lseek(fd, ((shdr64_t*)head->content)->shdr->sh_offset, SEEK_SET);
				write(fd, ((shdr64_t*)head->content)->mem, ((shdr64_t*)head->content)->shdr->sh_size);
			}
		}
		lseek(fd, bin->ehdr64->e_shoff, SEEK_SET);
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((shdr64_t*)head->content)->shdr, bin->ehdr64->e_shentsize);
			}
		}
	}
}
void	elftool_write32(elftool_t *bin, int fd)
{
	if (bin)
	{
		write(fd, bin->ehdr32, 0x40);
		lseek(fd, bin->ehdr32->e_phoff, SEEK_SET);
		for (t_list *head = bin->phdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((phdr32_t*)head->content)->phdr, bin->ehdr32->e_phentsize);
			}
		}
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				lseek(fd, ((shdr32_t*)head->content)->shdr->sh_offset, SEEK_SET);
				write(fd, ((shdr32_t*)head->content)->mem, ((shdr32_t*)head->content)->shdr->sh_size);
			}
		}
		lseek(fd, bin->ehdr32->e_shoff, SEEK_SET);
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((shdr32_t*)head->content)->shdr, bin->ehdr32->e_shentsize);
			}
		}
	}
}

int	elftool_write(elftool_t *bin, char *file_out)
{
	int r = 0;
	int fd = 0;

	if (!bin || !file_out)
	{
		r = -1;
	}
	else
	{
		fd = open(file_out, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
		if (fd < 0)
		{
			r = -1;
		}
		else
		{
			if (bin->elfclass == ELFCLASS32) {
				elftool_write32(bin, fd);
			}
			else {
				elftool_write64(bin, fd);
			}
			close(fd);
		}
	}
	return (r);
}
