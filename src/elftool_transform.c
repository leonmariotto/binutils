#include "elftool.h"
#include "elftool_transform.h"
#include <string.h>

int		nm_bin_transform(elftool_t *bin, elftool_transform_t *transform)
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

int		nm_bin_transform_codecave_injection(elftool_t *bin, elftool_transform_t *transform)
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

int		inject_shdr64(elftool_t *bin, elftool_transform_t *transform)
{
	int r = 0;
	uint64_t code_len_aligned = transform->code_len % 4096 == 0
		? transform->code_len
		: ((transform->code_len / 4096 + 1) * 4096);
	phdr64_t *last_ptload = NULL;
	shdr64_t *last_shdr = NULL;
	shdr64_t new_shdr_entry = {0};
	Elf64_Shdr *new_shdr = NULL;

	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	/* Find last PT_LOAD */
	for (t_list *head = bin->phdr ; head ; head = head->next) {
		if (((phdr64_t*)head->content)->phdr->p_type == PT_LOAD
				&& (!last_ptload || last_ptload->phdr->p_vaddr < ((phdr64_t*)head->content)->phdr->p_vaddr)) {
			last_ptload = (phdr64_t*)head->content;
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
			if (((shdr64_t*)head->content)->shdr->sh_offset
						> last_ptload->phdr->p_offset
				&& ((shdr64_t*)head->content)->shdr->sh_offset
						< last_ptload->phdr->p_offset + last_ptload->phdr->p_filesz
				&& (!last_shdr || last_shdr->shdr->sh_offset < ((shdr64_t*)head->content)->shdr->sh_offset)) {
				last_shdr = (shdr64_t*)head->content;
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
				if (head->content && ((shdr64_t*)head->content)->idx > last_shdr->idx) {
					((shdr64_t*)head->content)->idx += 1;
				}
				/* Update offset */
				if (head->content && ((shdr64_t*)head->content)->shdr->sh_offset
						> new_shdr->sh_offset) {
					fprintf(stderr, "%s:%d\n", __func__, __LINE__);
					((shdr64_t*)head->content)->shdr->sh_offset += code_len_aligned;
					((shdr64_t*)head->content)->shdr->sh_addr += code_len_aligned;
				}
			}
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			/* Add the new section in the section list */
			for (t_list *head = bin->shdr ; head && r == 0 ; head = head->next) {
				if (((shdr64_t*)head->content)->idx == last_shdr->idx) {
					t_list *new = ft_lstnew(&new_shdr_entry, sizeof(new_shdr_entry));
					if (!new) {
						r = -1;
					}
					else {
						new->next = head->next;
						head->next = new;
					}
				}
				if (((shdr64_t*)head->content)->shdr->sh_type == SHT_DYNSYM
						|| ((shdr64_t*)head->content)->shdr->sh_type == SHT_SYMTAB) {
					if (((shdr64_t*)head->content)->shdr->sh_link > new_shdr_entry.idx) {
						((shdr64_t*)head->content)->shdr->sh_link += 1;
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
				if (((phdr64_t*)head->content)->phdr->p_offset > last_ptload->phdr->p_offset
						+ (last_ptload->phdr->p_filesz - code_len_aligned)) {
					((phdr64_t*)head->content)->phdr->p_offset += code_len_aligned;
					((phdr64_t*)head->content)->phdr->p_vaddr += code_len_aligned;
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

int		nm_bin_transform_section_injection(elftool_t *bin, elftool_transform_t *transform)
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

int		nm_bin_transform_silvio_injection(elftool_t *bin, elftool_transform_t *transform)
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

