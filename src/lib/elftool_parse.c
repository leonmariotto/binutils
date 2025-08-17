#include "elftool.h"
#include <string.h>

static int elftool_parse64_ehdr(elftool_t *bin) {
  int r = 0;

  if (get_ehdr64(bin)->e_phoff > bin->length) {
    _error(r, "e_phoff outside the scope");
  }
  if (r == 0) {
    if (get_ehdr64(bin)->e_shoff > bin->length) {
        fprintf(stderr, "bin->length=%zu e_shoff=%lu\n", bin->length, get_ehdr64(bin)->e_shoff);
      _error(r, "e_shoff outside the scope");
    }
  }
  if (r == 0) {
    if (get_ehdr64(bin)->e_phentsize *
                get_ehdr64(bin)->e_phnum +
            get_ehdr64(bin)->e_phoff >
        bin->length) {
      _error(r, "phdr table run out of scope");
    }
  }
  if (r == 0) {
    if (get_ehdr64(bin)->e_shentsize *
                get_ehdr64(bin)->e_shnum +
            get_ehdr64(bin)->e_shoff >
        bin->length) {
      _error(r, "shdr table run out of scope");
    }
  }
  return (r);
}

static int elftool_parse32_ehdr(elftool_t *bin) {
  int r = 0;

  if (get_ehdr32(bin)->e_phoff > bin->length) {
    _error(r, "e_phoff outside the scope");
  }
  if (r == 0) {
    if (get_ehdr32(bin)->e_shoff > bin->length) {
      _error(r, "e_shoff outside the scope");
    }
  }
  if (r == 0) {
    if (get_ehdr32(bin)->e_phentsize *
                get_ehdr32(bin)->e_phnum +
            get_ehdr32(bin)->e_phoff >
        bin->length) {
      _error(r, "phdr table run out of scope");
    }
  }
  if (r == 0) {
    if (get_ehdr32(bin)->e_shentsize *
                get_ehdr32(bin)->e_shnum +
            get_ehdr32(bin)->e_shoff >
        bin->length) {
      _error(r, "shdr table run out of scope");
    }
  }
  return (r);
}

int elftool_parse_ehdr(elftool_t *bin) {
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
  } else if (bin->mem[offset] != ELFCLASS32 && bin->mem[offset] != ELFCLASS64) {
    _error(r, "invalid EI_CLASS");
  } else {
    /* 32bits program or 64bits program */
    bin->elfclass = bin->mem[offset++];
    if (bin->mem[offset] != ELFDATA2LSB && bin->mem[offset] != ELFDATA2MSB) {
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

int elftool_parse_phdr_32(elftool_t *bin)
{
  phdr32_t phdr = {0};
  list_t *new = NULL;
  uint32_t offset = get_ehdr32(bin)->e_phoff;
  uint16_t idx = 0;
  int r = 0;

  while (r == 0 && idx < get_ehdr32(bin)->e_phnum) {
    if (offset >= bin->length) {
      _error(r, "offset out of scope in phdr table");
    }
    Elf32_Phdr *phdr_ent = ((Elf32_Phdr *)&bin->mem[offset]);
    phdr.idx = idx;
    phdr.p_offset = offset;
    phdr.bin = bin;
    phdr.mem_offset = phdr_ent->p_offset;
    new = ft_lstnew(&phdr, sizeof(phdr));
    if (!new) {
      _error(r, "malloc error");
    }
    ft_lstpush(&bin->phdr, new);
    offset += get_ehdr32(bin)->e_phentsize;
    idx++;
  }
  return (r);
}

int elftool_parse_phdr_64(elftool_t *bin) {
  phdr64_t phdr = {0};
  list_t *new = NULL;
  uint64_t offset = get_ehdr64(bin)->e_phoff;
  uint16_t idx = 0;
  int r = 0;

  while (r == 0 && idx < get_ehdr64(bin)->e_phnum) {
    if (offset >= bin->length) {
      _error(r, "offset out of scope in phdr table");
    }
    Elf64_Phdr *phdr_ent = ((Elf64_Phdr *)&bin->mem[offset]);
    phdr.idx = idx;
    phdr.p_offset = offset;
    phdr.bin = bin;
    phdr.mem_offset = phdr_ent->p_offset;
    new = ft_lstnew(&phdr, sizeof(phdr));
    if (!new) {
      _error(r, "malloc error");
    }
    ft_lstpush(&bin->phdr, new);
    offset += get_ehdr64(bin)->e_phentsize;
    idx++;
  }
  return (r);
}

int elftool_parse_phdr(elftool_t *bin) {
  int r = 0;

  if (bin->elfclass == ELFCLASS32) {
    r = elftool_parse_phdr_32(bin);
  } else {
    r = elftool_parse_phdr_64(bin);
  }
  return (r);
}

int elftool_parse_shdr_32(elftool_t *bin) {
  list_t *head;
  shdr32_t shdr = {0};
  list_t *new = NULL;
  uint32_t offset = get_ehdr32(bin)->e_shoff;
  int symtabstrndx = -1;
  uint16_t idx = 0;
  int r = 0;

  if (!bin) {
    r = -1;
  } else {
    while (r == 0 && idx < get_ehdr32(bin)->e_shnum) {
      if (offset >= bin->length) {
        _error(r, "offset out of scope in shdr table");
      }
      Elf32_Shdr* shdr_ent = ((Elf32_Shdr *)&bin->mem[offset]);
      if (r == 0) {
        shdr.idx = idx;
        shdr.sh_offset = offset;
        shdr.mem_offset = shdr_ent->sh_offset;
        shdr.bin = bin;
      }
      if (r == 0) {
        new = ft_lstnew(&shdr, sizeof(shdr));
        if (!new) {
          _error(r, "malloc error");
        }
      }
      if (r == 0) {
        ft_lstpush(&bin->shdr, new);
        offset += get_ehdr32(bin)->e_shentsize;
        idx++;
        if (shdr_ent->sh_type == SHT_SYMTAB) {
          symtabstrndx = shdr_ent->sh_link;
        }
      }
    }
    if (symtabstrndx < 0) {
      _error(r, "symtab not found in sections");
    }
    if (r == 0) {
      head = bin->shdr;
      while (head) {
        if (get_shdr32_ent(head->content)->sh_type == SHT_STRTAB) {
          if (get_ehdr32(bin)->e_shstrndx == ((shdr32_t *)head->content)->idx) {
            bin->shstrtab32 = ((shdr32_t *)head->content);
          }
          if (symtabstrndx == ((shdr32_t *)head->content)->idx) {
            bin->strtab32 = ((shdr32_t *)head->content);
          }
        }
        head = head->next;
      }
    }
  }
  return (r);
}

int elftool_parse_shdr_64(elftool_t *bin) {
  list_t *head;
  shdr64_t shdr = {0};
  list_t *new = NULL;
  uint64_t offset = get_ehdr64(bin)->e_shoff;
  int symtabstrndx = -1;
  uint16_t idx = 0;
  int r = 0;

  if (!bin) {
    r = -1;
  } else {
    while (r == 0 && idx < get_ehdr64(bin)->e_shnum) {
      if (offset >= bin->length) {
        _error(r, "offset out of scope in shdr table");
      }
      Elf64_Shdr* shdr_ent = ((Elf64_Shdr *)&bin->mem[offset]);
      if (r == 0) {
        shdr.idx = idx;
        shdr.sh_offset = offset;
        shdr.mem_offset = shdr_ent->sh_offset;
        shdr.bin = bin;
      }
      if (r == 0) {
        new = ft_lstnew(&shdr, sizeof(shdr));
        if (!new) {
          _error(r, "malloc error");
        }
      }
      if (r == 0) {
        ft_lstpush(&bin->shdr, new);
        offset += get_ehdr64(bin)->e_shentsize;
        idx++;
        if (shdr_ent->sh_type == SHT_SYMTAB) {
          symtabstrndx = shdr_ent->sh_link;
        }
      }
    }
    if (symtabstrndx < 0) {
      _error(r, "symtab not found in sections");
    }
    if (r == 0) {
      head = bin->shdr;
      while (head) {
        if (get_shdr64_ent(head->content)->sh_type == SHT_STRTAB) {
          if (get_ehdr64(bin)->e_shstrndx == ((shdr64_t *)head->content)->idx) {
            bin->shstrtab64 = ((shdr64_t *)head->content);
          }
          if (symtabstrndx == ((shdr64_t *)head->content)->idx) {
            bin->strtab64 = ((shdr64_t *)head->content);
          }
        }
        head = head->next;
      }
    }
  }
  return (r);
}

int elftool_parse_shdr(elftool_t *bin) {
  int r = 0;

  if (bin->elfclass == ELFCLASS32) {
    r = elftool_parse_shdr_32(bin);
  } else {
    r = elftool_parse_shdr_64(bin);
  }
  return (r);
}

int elftool_parse_syms64(elftool_t *bin, void *symtab) {
  int r = 0;
  sym64_t syms = {0};
  list_t *new = NULL;
  uint16_t idx = 0;

  if (!bin || !symtab) {
    r = 0;
  } else {
    for (uint64_t sym_offset = 0;
         r == 0 && sym_offset < get_shdr64_ent(symtab)->sh_size;
         sym_offset += get_shdr64_ent(symtab)->sh_entsize) {

      uint64_t offset = sym_offset + get_shdr64_ent(symtab)->sh_offset;
      if (offset + sizeof(Elf64_Shdr) > bin->length) {
        _error(r, "offset out of scope in shdr table");
      }
      Elf64_Sym *sym_ent = (Elf64_Sym *)&bin->mem[offset];
      if (r == 0) {
        syms.idx = idx++;
        syms.sym_offset = offset;
        syms.mem_offset = sym_ent->st_value;
        syms.bin = bin;
        if (!syms.bin->strtab64) {
          _error(r, "no strtab found");
        }
        if (r == 0 && sym_ent->st_name + get_shdr64_ent(syms.bin->strtab64)->sh_offset >
                          bin->length) {
          _error(r, "symbol name value out of scope");
        } else {
          syms.name = strdup(
                 (char *)&bin->mem[get_shdr64_ent(bin->strtab64)->sh_offset + sym_ent->st_name]
          );
        }
      }
      if (r == 0) {
        for (list_t *head = bin->shdr; head->next; head = head->next) {
          if (((shdr64_t *)head->content)->idx == sym_ent->st_shndx) {
            syms.sh_offset = ((shdr64_t *)head->content)->sh_offset;
          }
        }
        new = ft_lstnew(&syms, sizeof(sym64_t));
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

int elftool_parse_syms32(elftool_t *bin, void *symtab) {
  int r = 0;
  sym32_t syms = {0};
  list_t *new = NULL;
  uint16_t idx = 0;

  if (!bin || !symtab) {
    r = 0;
  } else {
    for (uint32_t sym_offset = 0;
         r == 0 && sym_offset < get_shdr32_ent(symtab)->sh_size;
         sym_offset += get_shdr32_ent(symtab)->sh_entsize) {

      uint32_t offset = sym_offset + get_shdr32_ent(symtab)->sh_offset;
      if (offset + sizeof(Elf64_Shdr) > bin->length) {
        _error(r, "offset out of scope in shdr table");
      }
      Elf32_Sym *sym_ent = (Elf32_Sym *)&bin->mem[offset];
      if (r == 0) {
        syms.idx = idx++;
        syms.sym_offset = offset;
        syms.mem_offset = sym_ent->st_value;
        syms.bin = bin;
        if (!syms.bin->strtab32) {
          _error(r, "no strtab found");
        }
        if (r == 0 && sym_ent->st_name + get_shdr32_ent(syms.bin->strtab32)->sh_offset >
                          bin->length) {
          _error(r, "symbol name value out of scope");
        } else {
          syms.name = strdup(
                 (char *)&bin->mem[get_shdr32_ent(bin->strtab32)->sh_offset + sym_ent->st_name]
          );
        }
      }
      if (r == 0) {
        for (list_t *head = bin->shdr; head->next; head = head->next) {
          if (((shdr32_t *)head->content)->idx == sym_ent->st_shndx) {
            syms.sh_offset = ((shdr32_t *)head->content)->sh_offset;
          }
        }
        new = ft_lstnew(&syms, sizeof(sym32_t));
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

/*
 * @brief get symtab section
 * fill *symtab with either shdr32_t pointer or shdr64_t pointer.
 * @return      0 for success
 *              EINVAL
 * */
static int elftool_get_sym_sections(elftool_t *bin, void **symtab)
{
  int r = 0;

  if (!bin || !symtab)
      return EINVAL;
  for (list_t *head = bin->shdr; head; head = head->next) {

    if (bin->elfclass == ELFCLASS32) {
      if (head->content)
        ((shdr32_t *)head->content)->bin = bin;
      if (head->content &&
          get_shdr32_ent(head->content)->sh_type == SHT_SYMTAB) {
        *symtab = ((shdr32_t *)head->content);
      }
    } else {
      if (head->content)
        ((shdr64_t *)head->content)->bin = bin;
      if (head->content &&
          get_shdr64_ent(head->content)->sh_type == SHT_SYMTAB) {
        *symtab = ((shdr64_t *)head->content);
      }
    }
  }
  if (!*symtab) {
    printf("missing symbol table in section entry\n");
    r = EINVAL;
  }
  return (r);
}

int elftool_parse_syms(elftool_t *bin) {
  int r = 0;
  void *symtab = NULL;

  if (!bin) {
    r = -1;
  } else {
    r = elftool_get_sym_sections(bin, &symtab);
    if (r == 0) {
      if (bin->elfclass == ELFCLASS32) {
        r = elftool_parse_syms32(bin, symtab);
      } else {
        r = elftool_parse_syms64(bin, symtab);
      }
    }
  }
  return (r);
}

int elftool_parse(elftool_t *bin) {
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
