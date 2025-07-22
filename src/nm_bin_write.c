#include "nm_bin.h"


// TODO use fseek to replace ptr in file
// woody allow the use of fpusts, fflush, lseek
// or use an allocated space and one write, that's better but we don't know the size
void	nm_bin_write64(t_nm_bin *bin, int fd)
{
	if (bin)
	{
		write(fd, bin->ehdr64, 0x40);
		lseek(fd, bin->ehdr64->e_phoff, SEEK_SET);
		for (t_list *head = bin->phdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((t_nm_phdr64*)head->content)->phdr, bin->ehdr64->e_phentsize);
			}
		}
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				lseek(fd, ((t_nm_shdr64*)head->content)->shdr->sh_offset, SEEK_SET);
				write(fd, ((t_nm_shdr64*)head->content)->mem, ((t_nm_shdr64*)head->content)->shdr->sh_size);
			}
		}
		lseek(fd, bin->ehdr64->e_shoff, SEEK_SET);
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((t_nm_shdr64*)head->content)->shdr, bin->ehdr64->e_shentsize);
			}
		}
	}
}
void	nm_bin_write32(t_nm_bin *bin, int fd)
{
	if (bin)
	{
		write(fd, bin->ehdr32, 0x40);
		lseek(fd, bin->ehdr32->e_phoff, SEEK_SET);
		for (t_list *head = bin->phdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((t_nm_phdr32*)head->content)->phdr, bin->ehdr32->e_phentsize);
			}
		}
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				lseek(fd, ((t_nm_shdr32*)head->content)->shdr->sh_offset, SEEK_SET);
				write(fd, ((t_nm_shdr32*)head->content)->mem, ((t_nm_shdr32*)head->content)->shdr->sh_size);
			}
		}
		lseek(fd, bin->ehdr32->e_shoff, SEEK_SET);
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (head->content) {
				write(fd, ((t_nm_shdr32*)head->content)->shdr, bin->ehdr32->e_shentsize);
			}
		}
	}
}

int	nm_bin_write(t_nm_bin *bin, char *file_out)
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
				nm_bin_write32(bin, fd);
			}
			else {
				nm_bin_write64(bin, fd);
			}
			close(fd);
		}
	}
	return (r);
}
