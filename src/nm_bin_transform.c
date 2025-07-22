#include "nm_bin_transform.h"
#include <string.h>

int		nm_bin_transform(t_nm_bin *bin, t_nm_bin_transform *transform)
{
	int		r = 0;

	if (!bin || !transform)
	{
		r = -1;
	}
	else
	{
		if (transform->type == NM_CODECAVE_INJECT) {
			r = nm_bin_transform_codecave_injection(bin, transform);
		}
		else if (transform->type == NM_SILVIO_INJECT) {
			r = nm_bin_transform_silvio_injection(bin, transform);
		}
		else {
			r = nm_bin_transform_section_injection(bin, transform);
		}
	}
	return (r);
}

int		nm_bin_transform_codecave_injection(t_nm_bin *bin, t_nm_bin_transform *transform)
{
	int r = 0;

	if (!bin || !transform)
	{
		r = -1;
	}
	else
	{
		r = 0;
	}
	return (r);
}

int		inject_shdr64(t_nm_bin *bin, t_nm_bin_transform *transform)
{
	int r = 0;
	uint64_t code_len_aligned = transform->code_len % 4096 == 0
		? transform->code_len
		: ((transform->code_len / 4096 + 1) * 4096);
	t_nm_phdr64 *last_ptload = NULL;
	t_nm_shdr64 *last_shdr = NULL;
	t_nm_shdr64 new_shdr_entry = {0};
	Elf64_Shdr *new_shdr = NULL;

	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	/* Find last PT_LOAD */
	for (t_list *head = bin->phdr ; head ; head = head->next) {
		if (((t_nm_phdr64*)head->content)->phdr->p_type == PT_LOAD
				&& (!last_ptload || last_ptload->phdr->p_vaddr < ((t_nm_phdr64*)head->content)->phdr->p_vaddr)) {
			last_ptload = (t_nm_phdr64*)head->content;
		}
	}
	if (!last_ptload)
	{
		r = -1;
	}
	else
	{
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		/* Find last section for this segment */
		for (t_list *head = bin->shdr ; head ; head = head->next) {
			if (((t_nm_shdr64*)head->content)->shdr->sh_offset
						> last_ptload->phdr->p_offset
				&& ((t_nm_shdr64*)head->content)->shdr->sh_offset
						< last_ptload->phdr->p_offset + last_ptload->phdr->p_filesz
				&& (!last_shdr || last_shdr->shdr->sh_offset < ((t_nm_shdr64*)head->content)->shdr->sh_offset)) {
				last_shdr = (t_nm_shdr64*)head->content;
			}
		}
		if (!last_shdr) {
			r = -1;
		}
		if (r == 0)
		{
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
				/* Build new shdr */
				new_shdr = malloc(bin->ehdr64->e_shentsize);
				if (!new_shdr) {
					r = -1;
				}
		}
		if (r == 0)
		{
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			memset(new_shdr, 0, bin->ehdr64->e_shentsize);
			new_shdr->sh_type = SHT_PROGBITS;
			new_shdr->sh_flags = SHF_EXECINSTR;
			new_shdr->sh_addr = last_shdr->shdr->sh_offset + last_shdr->shdr->sh_size;
			new_shdr->sh_offset = last_shdr->shdr->sh_offset + last_shdr->shdr->sh_size;
			new_shdr->sh_size = transform->code_len;
			new_shdr->sh_link = 0;
			new_shdr->sh_info = 0;
			new_shdr->sh_addralign = 8;
			new_shdr->sh_entsize = 0;
			new_shdr_entry.shdr = new_shdr;
			new_shdr_entry.mem = transform->code;
			new_shdr_entry.idx = last_shdr->idx + 1;
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			for (t_list *head = bin->shdr ; head && r == 0 ; head = head->next) {
				/* Increase next idx */
				if (head->content && ((t_nm_shdr64*)head->content)->idx > last_shdr->idx) {
					((t_nm_shdr64*)head->content)->idx += 1;
				}
				/* Update offset */
				if (head->content && ((t_nm_shdr64*)head->content)->shdr->sh_offset
						> new_shdr->sh_offset) {
					fprintf(stderr, "%s:%d\n", __func__, __LINE__);
					((t_nm_shdr64*)head->content)->shdr->sh_offset += code_len_aligned;
					((t_nm_shdr64*)head->content)->shdr->sh_addr += code_len_aligned;
				}
			}
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			/* Add the new section in the section list */
			for (t_list *head = bin->shdr ; head && r == 0 ; head = head->next) {
				if (((t_nm_shdr64*)head->content)->idx == last_shdr->idx) {
					t_list *new = ft_lstnew(&new_shdr_entry, sizeof(new_shdr_entry));
					if (!new) {
						r = -1;
					}
					else {
						new->next = head->next;
						head->next = new;
					}
				}
				if (((t_nm_shdr64*)head->content)->shdr->sh_type == SHT_DYNSYM
						|| ((t_nm_shdr64*)head->content)->shdr->sh_type == SHT_SYMTAB) {
					if (((t_nm_shdr64*)head->content)->shdr->sh_link > new_shdr_entry.idx) {
						((t_nm_shdr64*)head->content)->shdr->sh_link += 1;
					}
				}
			}
		}
		if (r == 0)
		{
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			/* Update size and offset in phdr table */
			last_ptload->phdr->p_filesz += code_len_aligned;
			last_ptload->phdr->p_memsz += code_len_aligned;
			for (t_list *head = bin->phdr ; head ; head = head->next) {
				if (((t_nm_phdr64*)head->content)->phdr->p_offset > last_ptload->phdr->p_offset
						+ (last_ptload->phdr->p_filesz - code_len_aligned)) {
					((t_nm_phdr64*)head->content)->phdr->p_offset += code_len_aligned;
					((t_nm_phdr64*)head->content)->phdr->p_vaddr += code_len_aligned;
				}
			}
		}
		if (r == 0)
		{
			if (bin->ehdr64->e_shstrndx >= new_shdr_entry.idx) {
				bin->ehdr64->e_shstrndx += 1;
			}
			bin->ehdr64->e_shnum += 1;
		}
	}
	return (r);
}

int		nm_bin_transform_section_injection(t_nm_bin *bin, t_nm_bin_transform *transform)
{
	int r = 0;

	if (!bin || !transform)
	{
		r = -1;
	}
	else
	{
		if (bin->elfclass == ELFCLASS32) {
			r = 0;
		}
		else {
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			r = inject_shdr64(bin, transform);
		}
	}
	return (r);
}

int		nm_bin_transform_silvio_injection(t_nm_bin *bin, t_nm_bin_transform *transform)
{
	int r = 0;

	if (!bin || !transform)
	{
		r = -1;
	}
	else
	{
		r = 0;
	}
	return (r);
}

