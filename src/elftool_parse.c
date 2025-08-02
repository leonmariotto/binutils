#include "elftool.h"

static int	elftool_parse64_ehdr(elftool_t *bin)
{
	int r = 0;

	if (((Elf64_Ehdr*)bin->mem)->e_phoff > bin->length) {
		_error(r, "e_phoff outside the scope");
	}
	if (r == 0) {
		if (((Elf64_Ehdr*)bin->mem)->e_shoff > bin->length) {
			_error(r, "e_shoff outside the scope");
		}
	}
	if (r == 0) {
		if (((Elf64_Ehdr*)bin->mem)->e_phentsize * ((Elf64_Ehdr*)bin->mem)->e_phnum + ((Elf64_Ehdr*)bin->mem)->e_phoff > bin->length) {
			_error(r, "phdr table run out of scope");
		}
	}
	if (r == 0) {
		if (((Elf64_Ehdr*)bin->mem)->e_shentsize * ((Elf64_Ehdr*)bin->mem)->e_shnum + ((Elf64_Ehdr*)bin->mem)->e_shoff > bin->length) {
			_error(r, "shdr table run out of scope");
		}
	}
	if (r == 0) {
		bin->ehdr64 = (Elf64_Ehdr*)bin->mem;
	}
	return (r);
}

static int	elftool_parse32_ehdr(elftool_t *bin)
{
	int r = 0;

	if (((Elf32_Ehdr*)bin->mem)->e_phoff > bin->length) {
		_error(r, "e_phoff outside the scope");
	}
	if (r == 0) {
		if (((Elf32_Ehdr*)bin->mem)->e_shoff > bin->length) {
			_error(r, "e_shoff outside the scope");
		}
	}
	if (r == 0) {
		if (((Elf32_Ehdr*)bin->mem)->e_phentsize * ((Elf32_Ehdr*)bin->mem)->e_phnum + ((Elf32_Ehdr*)bin->mem)->e_phoff > bin->length) {
			_error(r, "phdr table run out of scope");
		}
	}
	if (r == 0) {
		if (((Elf32_Ehdr*)bin->mem)->e_shentsize * ((Elf32_Ehdr*)bin->mem)->e_shnum + ((Elf32_Ehdr*)bin->mem)->e_shoff > bin->length) {
			_error(r, "shdr table run out of scope");
		}
	}
	if (r == 0) {
		bin->ehdr32 = (Elf32_Ehdr*)bin->mem;
	}
	return (r);
}

int	elftool_parse_ehdr(elftool_t *bin)
{
	int offset = 0;
	int r = 0;

	if (!bin || !bin->mem) {
		_error(r, "Internal Error");
	} else if (bin->length < 0x40) {
		_error(r, "Invalid length (<0x40)");
	} else if (bin->mem[offset++] != 0x7f) {
		_error(r, "Invalid magic numer");
	} else if (bin->mem[offset++] != 0x45) {
		_error(r, "Invalid magic numer");
	} else if (bin->mem[offset++] != 0x4c) {
		_error(r, "Invalid magic numer");
	} else if (bin->mem[offset++] != 0x46) {
		_error(r, "Invalid magic numer");
	} else if (bin->mem[offset] != ELFCLASS32
		&& bin->mem[offset] != ELFCLASS64) {
		_error(r, "invalid EI_CLASS");
	} else {
		/* 32bits program or 64bits program */
		bin->elfclass = bin->mem[offset++];
		if (bin->mem[offset] != ELFDATA2LSB
			&& bin->mem[offset] != ELFDATA2MSB) {
			_error(r, "invalid EI_DATA");
		}
	}
	if (r == 0) {
		/* Endianness, affect fields starting from 0x10 */
		bin->endian = bin->mem[offset++];
		if (bin->mem[offset] > 0x12) {
			_error(r, "invalid EI_OSABI");
		}
	}
	if (r == 0) {
		/* It is class specific from 0x14 */
		if (bin->elfclass == ELFCLASS32) {
			r = elftool_parse32_ehdr(bin);
		} else {
			r = elftool_parse64_ehdr(bin);
		}
	}
	return (r);
}
int	elftool_parse_phdr_32(elftool_t *bin)
{
	phdr32_t	phdr = {0};
	t_list		*new = NULL;
	uint32_t	offset = bin->ehdr32->e_phoff;
	uint16_t	idx = 0;
	int		r = 0;

	while (r == 0 && idx < bin->ehdr32->e_phnum)
	{
		if (offset >= bin->length) {
			_error(r, "offset out of scope in phdr table");
		}
		phdr.idx = idx;
		phdr.phdr = ((Elf32_Phdr*)&bin->mem[offset]);
		phdr.bin = bin;
		new = ft_lstnew(&phdr, sizeof(phdr));
		if (!new) {
			_error(r, "malloc error");
		}
		ft_lstpush(&bin->phdr, new);
		offset += bin->ehdr32->e_phentsize;
		idx++;
	}
	return (r);
}

int	elftool_parse_phdr_64(elftool_t *bin)
{
	phdr64_t	phdr = {0};
	t_list		*new = NULL;
	uint64_t	offset = bin->ehdr64->e_phoff;
	uint16_t	idx = 0;
	int		r = 0;

	while (r == 0 && idx < bin->ehdr64->e_phnum)
	{
		if (offset >= bin->length) {
			_error(r, "offset out of scope in phdr table");
		}
		phdr.idx = idx;
		phdr.phdr = ((Elf64_Phdr*)&bin->mem[offset]);
		phdr.bin = bin;
		new = ft_lstnew(&phdr, sizeof(phdr));
		if (!new) {
			_error(r, "malloc error");
		}
		ft_lstpush(&bin->phdr, new);
		offset += bin->ehdr64->e_phentsize;
		idx++;
	}
	return (r);
}

int	elftool_parse_phdr(elftool_t *bin)
{
	int r = 0;

	if (bin->elfclass == ELFCLASS32) {
		r = elftool_parse_phdr_32(bin);
	}
	else {
		r = elftool_parse_phdr_64(bin);
	}
	return (r);
}

int	elftool_parse_shdr_32(elftool_t *bin)
{
	t_list *head;
	shdr32_t	shdr = {0};
	t_list		*new = NULL;
	uint32_t	offset = bin->ehdr32->e_shoff;
	int			symtabstrndx = -1;
	uint16_t	idx = 0;
	int			r = 0;

	if (!bin || !bin->ehdr32)
	{
		r = -1;
	}
	else
	{
		while (r == 0 && idx < bin->ehdr32->e_shnum)
		{
			if (offset >= bin->length) {
				_error(r, "offset out of scope in shdr table");
			}
			if (r == 0)
			{
				shdr.idx = idx;
				shdr.shdr = ((Elf32_Shdr*)&bin->mem[offset]);
				shdr.mem = &bin->mem[shdr.shdr->sh_offset];
				shdr.bin = bin;
			}
			if (r == 0)
			{
				new = ft_lstnew(&shdr, sizeof(shdr));
				if (!new) {
					_error(r, "malloc error");
				}
			}
			if (r == 0)
			{
				ft_lstpush(&bin->shdr, new);
				offset += bin->ehdr32->e_shentsize;
				idx++;
				if (shdr.shdr->sh_type == SHT_SYMTAB) {
					symtabstrndx = shdr.shdr->sh_link;
				}
			}
		}
		if (symtabstrndx < 0) {
			_error(r, "symtab not found in sections");
		}
		if (r == 0)
		{
			head = bin->shdr;
			while (head)
			{
				if (((shdr32_t*)head->content)->shdr->sh_type == SHT_STRTAB)
				{
					if (bin->ehdr32->e_shstrndx == ((shdr32_t*)head->content)->idx) {
						bin->shstrtab32 = ((shdr32_t*)head->content);
					}
					if (symtabstrndx == ((shdr32_t*)head->content)->idx) {
						bin->strtab32 = ((shdr32_t*)head->content);
					}
				}
				head = head->next;
			}
		}
	}
	return (r);
}

int	elftool_parse_shdr_64(elftool_t *bin)
{
	t_list *head;
	shdr64_t	shdr = {0};
	t_list		*new = NULL;
	uint64_t	offset = bin->ehdr64->e_shoff;
	int			symtabstrndx = -1;
	uint16_t	idx = 0;
	int			r = 0;

	if (!bin || !bin->ehdr64)
	{
		r = -1;
	}
	else
	{
		while (r == 0 && idx < bin->ehdr64->e_shnum)
		{
			if (offset >= bin->length) {
				_error(r, "offset out of scope in shdr table");
			}
			if (r == 0)
			{
				shdr.idx = idx;
				shdr.shdr = ((Elf64_Shdr*)&bin->mem[offset]);
				shdr.mem = &bin->mem[shdr.shdr->sh_offset];
				shdr.bin = bin;
			}
			if (r == 0)
			{
				new = ft_lstnew(&shdr, sizeof(shdr));
				if (!new) {
					_error(r, "malloc error");
				}
			}
			if (r == 0)
			{
				ft_lstpush(&bin->shdr, new);
				if (shdr.shdr->sh_type == SHT_SYMTAB) {
					symtabstrndx = shdr.shdr->sh_link;
				}
				offset += bin->ehdr64->e_shentsize;
				idx++;
			}
		}
		if (symtabstrndx < 0) {
			_error(r, "symtab not found in sections");
		}
		if (r == 0)
		{
			head = bin->shdr;
			while (head)
			{
				if (((shdr64_t*)head->content)->shdr->sh_type == SHT_STRTAB)
				{
					if (bin->ehdr64->e_shstrndx == ((shdr64_t*)head->content)->idx) {
						bin->shstrtab64 = ((shdr64_t*)head->content);
					}
					if (symtabstrndx == ((shdr64_t*)head->content)->idx) {
						bin->strtab64 = ((shdr64_t*)head->content);
					}
				}
				head = head->next;
			}
		}
	}
	return (r);
}

int	elftool_parse_shdr(elftool_t *bin)
{
	int r = 0;

	if (bin->elfclass == ELFCLASS32) {
		r = elftool_parse_shdr_32(bin);
	}
	else {
		r = elftool_parse_shdr_64(bin);
	}
	return (r);
}

int		elftool_parse_syms64(elftool_t *bin, void *symtab)
{
	int r = 0;
	syms64_t	syms = {0};
	t_list		*new = NULL;
	uint16_t	idx = 0;

	if (!bin || !symtab)
	{
		r = 0;
	}
	else
	{
		for (uint64_t sym_offset = 0
				; r == 0 && sym_offset < ((Elf64_Shdr*)symtab)->sh_size
				; sym_offset += ((Elf64_Shdr*)symtab)->sh_entsize)
		{
			uint64_t offset = sym_offset + ((Elf64_Shdr*)symtab)->sh_offset;
			if (offset + sizeof(Elf64_Shdr) > bin->length) {
				_error(r, "offset out of scope in shdr table");
			}
			if (r == 0)
			{
				syms.idx = idx++;
				syms.syms = (Elf64_Sym*)&bin->mem[offset];
				syms.bin = bin;
				if (!syms.bin->strtab64)
				{
					_error(r, "no strtab found");
				}
				if (r == 0 && syms.syms->st_name + syms.bin->strtab64->shdr->sh_offset > bin->length) {
					_error(r, "symbol name value out of scope");
				}
			}
			if (r == 0)
			{
				for (t_list *head = bin->shdr ; head->next ; head = head->next) {
					if (((shdr64_t*)head->content)->idx == syms.syms->st_shndx)
					{
						syms.shdr = ((shdr64_t*)head->content)->shdr;
					}
				}
				new = ft_lstnew(&syms, sizeof(syms64_t));
				if (!new) {
					_error(r, "malloc error");
				} else {
					ft_lstpush(&bin->syms, new);
				}
			}
		}
	}
	return (r);
}

int		elftool_parse_syms32(elftool_t *bin, void *symtab)
{
	int r = 0;
	syms32_t	syms = {0};
	t_list		*new = NULL;
	uint16_t	idx = 0;

	if (!bin || !symtab)
	{
		r = 0;
	}
	else
	{
		for (uint32_t sym_offset = 0
				; r == 0 && sym_offset < ((Elf32_Shdr*)symtab)->sh_size
				; sym_offset += ((Elf32_Shdr*)symtab)->sh_entsize)
		{
			uint32_t offset = sym_offset + ((Elf32_Shdr*)symtab)->sh_offset;
			if (offset + sizeof(Elf64_Shdr) > bin->length) {
				_error(r, "offset out of scope in shdr table");
			}
			if (r == 0)
			{
				syms.idx = idx++;
				syms.syms = (Elf32_Sym*)&bin->mem[offset];
				syms.bin = bin;
				if (!syms.bin->strtab32)
				{
					_error(r, "no strtab found");
				}
				if (r == 0 && syms.syms->st_name + syms.bin->strtab32->shdr->sh_offset > bin->length) {
					_error(r, "symbol name value out of scope");
				}
			}
			if (r == 0)
			{
				for (t_list *head = bin->shdr ; head->next ; head = head->next) {
					if (((shdr32_t*)head->content)->idx == syms.syms->st_shndx)
					{
						syms.shdr = ((shdr32_t*)head->content)->shdr;
					}
				}
				new = ft_lstnew(&syms, sizeof(syms32_t));
				if (!new) {
					_error(r, "malloc error");
				} else {
					ft_lstpush(&bin->syms, new);
				}
			}
		}
	}
	return (r);
}

static int	elftool_get_sym_sections(elftool_t *bin, void **symtab)
{
	int r = 0;

	for (t_list *head = bin->shdr ; head ; head = head->next) {
		if (bin->elfclass == ELFCLASS32) {
			if (head->content && ((shdr32_t*)head->content)->shdr->sh_type == SHT_SYMTAB) {
				*symtab = ((shdr32_t*)head->content)->shdr;
			}
		}
		else {
			if (head->content && ((shdr64_t*)head->content)->shdr->sh_type == SHT_SYMTAB) {
				*symtab = ((shdr64_t*)head->content)->shdr;
			}
		}
	}
	if (!symtab)
	{
		printf("missing symbol table in section entry\n");
		r = -1;
	}
	return (r);
}

int	elftool_parse_syms(elftool_t *bin)
{
	int r = 0;
	void *symtab = NULL;

	if (!bin)
	{
		r = -1;
	}
	else
	{
		r = elftool_get_sym_sections(bin, &symtab);
		if (r == 0) {
			if (bin->elfclass == ELFCLASS32) {
				r = elftool_parse_syms32(bin, symtab);
			}
			else {
				r = elftool_parse_syms64(bin, symtab);
			}
		}
	}
	return (r);
}

int	elftool_parse(elftool_t *bin)
{
	int r = 0;

	if (r == 0) {
		r = elftool_parse_ehdr(bin);
	}
	if (r == 0) {
		r = elftool_parse_phdr(bin);
	}
	if (r == 0) {
		r = elftool_parse_shdr(bin);
	}
	if (r == 0) {
		r = elftool_parse_syms(bin);
	}
	return (r);
}

