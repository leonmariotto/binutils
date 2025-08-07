#include "elftool.h"
#include <elf.h>
#include <stdio.h>
#include <string.h>

extern elftool_t bin;

void elftool_dump_ehdr(elftool_t *bin) {
  printf("===== ELF HEADER\n");
  if (bin->elfclass == ELFCLASS32) {
    printf("Magic number and other info = %x %x %x %x\n",
           ((Elf32_Ehdr *)bin->mem)->e_ident[0],
           ((Elf32_Ehdr *)bin->mem)->e_ident[1],
           ((Elf32_Ehdr *)bin->mem)->e_ident[2],
           ((Elf32_Ehdr *)bin->mem)->e_ident[3]);
    printf("Object file type = %hx\n", ((Elf32_Ehdr *)bin->mem)->e_type);
    printf("Architecture = %hx\n", ((Elf32_Ehdr *)bin->mem)->e_machine);
    printf("Object file version = %x\n", ((Elf32_Ehdr *)bin->mem)->e_version);
    printf("Entry point virtual address = %x\n",
           ((Elf32_Ehdr *)bin->mem)->e_entry);
    printf("Program header table file offset = %x\n",
           ((Elf32_Ehdr *)bin->mem)->e_phoff);
    printf("Section header table file offset = %x\n",
           ((Elf32_Ehdr *)bin->mem)->e_shoff);
    printf("Processor-specific flags = %x\n",
           ((Elf32_Ehdr *)bin->mem)->e_flags);
    printf("ELF header size in bytes = %hx\n",
           ((Elf32_Ehdr *)bin->mem)->e_ehsize);
    printf("Program header table entry size = %hx\n",
           ((Elf32_Ehdr *)bin->mem)->e_phentsize);
    printf("Program header table entry count = %hx\n",
           ((Elf32_Ehdr *)bin->mem)->e_phnum);
    printf("Section header table entry size = %hx\n",
           ((Elf32_Ehdr *)bin->mem)->e_shentsize);
    printf("Section header table entry count = %hx\n",
           ((Elf32_Ehdr *)bin->mem)->e_shnum);
    printf("Section header string table index = %hx\n",
           ((Elf32_Ehdr *)bin->mem)->e_shstrndx);
  } else {
    printf("Magic number and other info = %x %x %x %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_ident[0],
           ((Elf64_Ehdr *)bin->mem)->e_ident[1],
           ((Elf64_Ehdr *)bin->mem)->e_ident[2],
           ((Elf64_Ehdr *)bin->mem)->e_ident[3]);
    printf("Object file type = %hx\n", ((Elf64_Ehdr *)bin->mem)->e_type);
    printf("Architecture = %hx\n", ((Elf64_Ehdr *)bin->mem)->e_machine);
    printf("Object file version = %x\n", ((Elf64_Ehdr *)bin->mem)->e_version);
    printf("Entry point virtual address = %lx\n",
           ((Elf64_Ehdr *)bin->mem)->e_entry);
    printf("Program header table file offset = %lx\n",
           ((Elf64_Ehdr *)bin->mem)->e_phoff);
    printf("Section header table file offset = %lx\n",
           ((Elf64_Ehdr *)bin->mem)->e_shoff);
    printf("Processor-specific flags = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_flags);
    printf("ELF header size in bytes = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_ehsize);
    printf("Program header table entry size = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_phentsize);
    printf("Program header table entry count = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_phnum);
    printf("Section header table entry size = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_shentsize);
    printf("Section header table entry count = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_shnum);
    printf("Section header string table index = %x\n",
           ((Elf64_Ehdr *)bin->mem)->e_shstrndx);
  }
  printf("\n");
}

/*
** SEC_HAS_CONTENT : hdr->sh_type != SHT_NOBITS
** SEC_READONLY : ((hdr->sh_flags & SHF_WRITE) == 0)
** SEC_DATA : ((flags & SEC_LOAD) != 0 && ((hdr->sh_flags & SHF_EXECINSTR) ==
*0))
** SEC_CODE : ((hdr->sh_flags & SHF_EXECINSTR) != 0)
*/

static char strcmp_nm(char *min, char *cur) {
  /* unset LANG to avoid a weird sort from nm */
  return (strcmp(min, cur));
}

static int sort_symbols64(elftool_t *bin) {
  t_list *head = NULL;
  t_list *new;
  t_list *newList = NULL;
  const char ff[32] = {127};
  char *min = (char *)ff;
  char *cur = NULL;
  int index;
  int indexSelected = 0;

  // remove the first indice
  ft_lstdelone(&bin->syms, default_del);
  while (bin->syms && (ELF64_ST_TYPE(((syms64_t *)bin->syms->content)->syms->st_info) ==
             STT_SECTION ||
         ELF64_ST_TYPE(((syms64_t *)bin->syms->content)->syms->st_info) ==
             STT_FILE)) {
    ft_lstdelone(&bin->syms, default_del);
  }
  head = bin->syms;
  if (head) {
    while (head->next) {
      if (ELF64_ST_TYPE(((syms64_t *)head->next->content)->syms->st_info) ==
              STT_SECTION ||
          ELF64_ST_TYPE(((syms64_t *)head->next->content)->syms->st_info) ==
              STT_FILE) {
        ft_lstdelone(&head->next, default_del);
      }
      head = head->next;
    }
  }
  while (bin->syms) {
    new = NULL;
    head = bin->syms;
    /* Iterate over initial list , find the index of the lower */
    index = 0;
    min = (char *)ff;
    while (head) {
      fprintf(stderr, "sort_symbols64: head=%p\n", head);
      cur = (char *)&((syms64_t *)head->content)
                ->bin->mem[((syms64_t *)head->content)
                               ->bin->strtab64->shdr->sh_offset +
                           ((syms64_t *)head->content)->syms->st_name];
      fprintf(stderr, "sort_symbols64: cur=%s\n", cur);
      if (strcmp_nm(min, cur) > 0) {
        min = cur;
        indexSelected = index;
      }
      index++;
      head = head->next;
    }
    // fprintf(stderr, "final min={%s}\n", min);
    /* When lower found, get it out from the initial list */
    if (indexSelected == 0) {
      new = ft_lstnew(bin->syms->content, bin->syms->content_size);
      ft_lstdelone(&bin->syms, default_del);
    } else {
      index = 0;
      head = bin->syms;
      while (head) {
        if (index == indexSelected - 1) {
          if (!head->next) {
            fprintf(stderr, "Weird case index=%d!\n", index);
          } else {
            new = ft_lstnew(head->next->content, head->next->content_size);
            ft_lstdelone(&head->next, default_del);
          }
          break;
        }
        head = head->next;
        index++;
      }
    }
    /* Now push the new element into the new list */
    if (new) {
      ft_lstpush(&newList, new);
    } else {
      fprintf(stderr, "Weird case follow!\n");
      ft_lstdelone(&bin->syms, default_del);
    }
  }
  bin->syms = newList;
  // ft_lstrev(&newList);
  return (0);
}

static int sort_symbols32(elftool_t *bin) {
  t_list *head = NULL;
  t_list *new;
  t_list *newList = NULL;
  const char ff[32] = {127};
  char *min = (char *)ff;
  char *cur = NULL;
  int index;
  int indexSelected = 0;

  // remove the first indice
  ft_lstdelone(&bin->syms, default_del);
  while (bin->syms && (ELF32_ST_TYPE(((syms32_t *)bin->syms->content)->syms->st_info) ==
             STT_SECTION ||
         ELF32_ST_TYPE(((syms32_t *)bin->syms->content)->syms->st_info) ==
             STT_FILE)) {
    ft_lstdelone(&bin->syms, default_del);
  }
  head = bin->syms;
  if (head) {
    while (head->next) {
      if (ELF32_ST_TYPE(((syms32_t *)head->next->content)->syms->st_info) ==
              STT_SECTION ||
          ELF32_ST_TYPE(((syms32_t *)head->next->content)->syms->st_info) ==
              STT_FILE) {
        ft_lstdelone(&head->next, default_del);
      }
      head = head->next;
    }
  }
  while (bin->syms) {
    new = NULL;
    head = bin->syms;
    /* Iterate over initial list , find the index of the lower */
    index = 0;
    min = (char *)ff;
    while (head) {
      cur = (char *)&((syms32_t *)head->content)
                ->bin->mem[((syms32_t *)head->content)
                               ->bin->strtab32->shdr->sh_offset +
                           ((syms32_t *)head->content)->syms->st_name];
      // fprintf(stderr, "cur={%s} min={%s} st_name=%d\n", cur, min,
      // ((syms32_t*)head->content)->syms->st_name);
      if (strcmp_nm(min, cur) > 0) {
        min = cur;
        indexSelected = index;
      }
      index++;
      head = head->next;
    }
    // fprintf(stderr, "final min={%s}\n", min);
    /* When lower found, get it out from the initial list */
    if (indexSelected == 0) {
      new = ft_lstnew(bin->syms->content, bin->syms->content_size);
      ft_lstdelone(&bin->syms, default_del);
    } else {
      index = 0;
      head = bin->syms;
      while (head) {
        if (index == indexSelected - 1) {
          if (!head->next) {
            fprintf(stderr, "Weird case index=%d!\n", index);
          } else {
            new = ft_lstnew(head->next->content, head->next->content_size);
            ft_lstdelone(&head->next, default_del);
          }
          break;
        }
        head = head->next;
        index++;
      }
    }
    /* Now push the new element into the new list */
    if (new) {
      ft_lstpush(&newList, new);
    } else {
      fprintf(stderr, "Weird case follow!\n");
      ft_lstdelone(&bin->syms, default_del);
    }
  }
  bin->syms = newList;
  // ft_lstrev(&newList);
  return (0);
}

static char nm_flag_magic(uint8_t type, uint8_t bind, uint64_t sh_type,
                          uint64_t sh_flags, uint16_t st_shndx) {
  char nm_id;

  if (st_shndx == SHN_ABS) {
    /* The symbol value is absolute */
    nm_id = (bind == STB_GLOBAL ? 'A' : 'a');
  } else if (type == STT_GNU_IFUNC) {
    /* The symbol is an indirect function, GNU extension */
    nm_id = 'i';
  } else if (bind == STB_GNU_UNIQUE) {
    /* The symbol is a unique global symbol, GNU extension */
    nm_id = 'u';
  } else if (bind == STB_WEAK && sh_type && type == STT_OBJECT) {
    /* The symbol is a weak object, uppercase means default value has been
     * specified */
    nm_id = (st_shndx == SHN_UNDEF ? 'v' : 'V');
  } else if (bind == STB_WEAK) {
    /* The symbol is a weak object not specifically tagged as a weak object */
    nm_id = (st_shndx == SHN_UNDEF ? 'w' : 'W');
  }
  /* If in section unused */
  else if (sh_type == SHT_NULL) {
    /* The symbol is not defined */
    nm_id = 'U';
  } else if (sh_type == SHT_X86_64_UNWIND) {
    /* The symbol is in a stack unwind section */
    nm_id = 'p';
  } else if (
      //		(!ft_memcmp(sh_name, ".rodata", 8)
      //		|| !ft_memcmp(sh_name, ".group", 7))
      sh_type != SHT_NOBITS && (sh_flags & SHF_ALLOC) == 0 &&
      (sh_flags & ~(SHF_MASKOS | SHF_MERGE | SHF_STRINGS | SHF_ALLOC |
                    SHF_INFO_LINK)) == 0) {
    nm_id = 'n';
  }
  /* If its not in writable and executable and occupy ram, it's readonly */
  else if ((sh_flags & SHF_EXECINSTR) == 0 && (sh_flags & SHF_WRITE) == 0 &&
           sh_type != SHT_NOBITS &&
           (sh_flags & ~(SHF_MASKOS | SHF_MERGE | SHF_STRINGS | SHF_ALLOC |
                         SHF_INFO_LINK)) == 0) {
    /* The symbol is in a read-only data section */
    nm_id = (bind == STB_GLOBAL ? 'R' : 'r');
  } else if ((((sh_flags & 0x03) == 0x03) && sh_type == SHT_PROGBITS) ||
             sh_type == SHT_DYNAMIC || sh_type == SHT_FINI_ARRAY ||
             sh_type == SHT_INIT_ARRAY || sh_type == SHT_PREINIT_ARRAY) {
    /* The symbol is in initialized data section */
    nm_id = (bind == STB_GLOBAL ? 'D' : 'd');
  } else if (sh_type == SHT_NOBITS) {
    /* The symbol is in bss data section */
    nm_id = (bind == STB_GLOBAL ? 'B' : 'b');
  } else if (sh_type == SHT_PROGBITS) {
    /* The symbol is in the text section */
    nm_id = (bind == STB_GLOBAL ? 'T' : 't');
  } else {
    nm_id = '?';
  }
  return (nm_id);
}

void elftool_dump_nm32(t_list *lst_syms) {
  syms32_t *syms = (syms32_t *)lst_syms->content;
  uint8_t type = ELF32_ST_TYPE(syms->syms->st_info);
  uint8_t bind = ELF32_ST_BIND(syms->syms->st_info);
  char nm_id = '?';

  /* Don't dump section and file symbols */
  if (type != STT_SECTION && type != STT_FILE) {
    // TODO common symbol 'c'  and initialized data section for small object 'g'
    // TODO debugging symbols 'N', indirect reference 'I' and 'i'
    /* Don't dump addr if symbol is in a section unused or in a special section
     */
    // if (syms->shdr->sh_type != SHT_NULL) {
    if (syms->shdr->sh_type != SHT_NULL || syms->syms->st_shndx == SHN_ABS) {
      printf("%08x", syms->syms->st_value);
    } else {
      printf("        ");
    }
    nm_id = nm_flag_magic(type, bind, syms->shdr->sh_type, syms->shdr->sh_flags,
                          syms->syms->st_shndx);
    printf(" %c ", nm_id);
    if (syms->bin->strtab32->shdr->sh_offset + syms->syms->st_name >
        syms->bin->length) {
      fprintf(stderr, "\nERROR: symbol string out of mem\n");
    } else {
      printf("%s\n",
             (char *)&syms->bin->mem[syms->bin->strtab32->shdr->sh_offset +
                                     syms->syms->st_name]);
    }
  }
}

void elftool_dump_nm64(t_list *lst_syms) {
  syms64_t *syms = (syms64_t *)lst_syms->content;
  uint8_t type = ELF32_ST_TYPE(syms->syms->st_info);
  uint8_t bind = ELF32_ST_BIND(syms->syms->st_info);
  char nm_id = '?';

  /* Don't dump section and file symbols */
  if (type != STT_SECTION && type != STT_FILE) {
    // TODO common symbol 'c'  and initialized data section for small object 'g'
    // TODO debugging symbols 'N', indirect reference 'I' and 'i'
    /* Don't dump addr if symbol is in a section unused or in a special section
     */
    if (syms->shdr->sh_type != SHT_NULL || syms->syms->st_shndx == SHN_ABS) {
      printf("%016lx", syms->syms->st_value);
    } else {
      printf("                ");
    }
    nm_id = nm_flag_magic(type, bind, syms->shdr->sh_type, syms->shdr->sh_flags,
                          syms->syms->st_shndx);
    printf(" %c ", nm_id);
    if (syms->bin->strtab64->shdr->sh_offset + syms->syms->st_name >
        syms->bin->length) {
      fprintf(stderr, "\nERROR: symbol string out of mem\n");
    } else {
      printf("%s\n",
             (char *)&syms->bin->mem[syms->bin->strtab64->shdr->sh_offset +
                                     syms->syms->st_name]);
    }
  }
}

void elftool_dump_nm(elftool_t *bin) {
  if (bin->elfclass == ELFCLASS32) {
    /* Ignore first symbole always NULL */

    sort_symbols32(bin);
    if (bin->syms) {
      ft_lstiter(bin->syms, elftool_dump_nm32);
    } else {
      fprintf(stderr, "elftool: %s: no symbols\n", bin->path);
    }
  } else {
    /* Ignore first symbole always NULL */
    if (bin->syms) {
      sort_symbols64(bin);
      // ft_lstiter(bin->syms->next, elftool_dump_nm64);
      ft_lstiter(bin->syms, elftool_dump_nm64);
    } else {
      fprintf(stderr, "elftool: %s: no symbols\n", bin->path);
    }
  }
}

void elftool_dump_phdr32(t_list *lst_phdr) {
  phdr32_t *phdr = NULL;

  if (lst_phdr) {
    phdr = (phdr32_t *)lst_phdr->content;
  }
  if (phdr) {
    printf("%3d ", phdr->idx);
    // uint8_t buf[8] = {0};
    // char buf_flag[8] = "NLDINSPT";
    // for (int i  = 0 ; i < 7 ; i++) {
    //	buf[i] = ((phdr->phdr->p_type & (1 << i)) ? buf_flag[i] : ' ');
    // }
    char *buf_flag[8] = {"NULL", "LOAD",  "DYNAMIC", "INTERP",
                         "NOTE", "SHLIB", "PHDR",    "TLS"};
    printf("%8.8s ", buf_flag[phdr->phdr->p_type & 0x0F]);
    printf("%8x ", phdr->phdr->p_flags);
    printf("%8x ", phdr->phdr->p_offset);
    printf("%8x ", phdr->phdr->p_vaddr);
    printf("%8x ", phdr->phdr->p_filesz);
    printf("%8x ", phdr->phdr->p_memsz);
    printf("%6x\n", phdr->phdr->p_align);
  }
}

void elftool_dump_phdr64(t_list *lst_phdr) {
  phdr64_t *phdr = NULL;

  if (lst_phdr) {
    phdr = (phdr64_t *)lst_phdr->content;
  }
  if (phdr) {
    printf("%3d ", phdr->idx);
    // uint8_t buf[8] = {0};
    // char buf_flag[8] = "NLDINSPT";
    // for (int i  = 0 ; i < 7 ; i++) {
    //	buf[i] = ((phdr->phdr->p_type & (1 << i)) ? buf_flag[i] : ' ');
    // }
    char *buf_flag[8] = {"NULL", "LOAD",  "DYNAMIC", "INTERP",
                         "NOTE", "SHLIB", "PHDR",    "TLS"};
    if ((phdr->phdr->p_type & 0x60000000) != 0) {
      printf("%8.8s ", "OS_SPEC");
    } else if ((phdr->phdr->p_type & 0x70000000) != 0) {
      printf("%8.8s ", "PROC_SPEC");
    } else {
      printf("%8.8s ", buf_flag[phdr->phdr->p_type & 0x0F]);
    }
    printf("%8x ", phdr->phdr->p_flags);
    printf("%16lx ", phdr->phdr->p_offset);
    printf("%16lx ", phdr->phdr->p_vaddr);
    printf("%16lx ", phdr->phdr->p_filesz);
    printf("%16lx ", phdr->phdr->p_memsz);
    printf("%6lx\n", phdr->phdr->p_align);
  }
}

void elftool_dump_phdr(elftool_t *bin) {
  if (bin) {
    printf("===== PROGRAM HEADER\n");
    if (bin->elfclass == ELFCLASS32) {
      if (bin->phdr) {
        printf("%3s %8s %8s %8s %8s %8s %8s %6s\n", "id", "type", "flags",
               "offset", "vaddr", "filesz", "memsz", "align");
        ft_lstiter(bin->phdr, elftool_dump_phdr32);
      } else {
        fprintf(stderr, "elftool: %s: no program header\n", bin->path);
      }
    } else {
      if (bin->phdr) {
        printf("%3s %8s %8s %16s %16s %16s %16s %6s\n", "id", "type", "flags",
               "offset", "vaddr", "filesz", "memsz", "align");
        ft_lstiter(bin->phdr, elftool_dump_phdr64);
      } else {
        fprintf(stderr, "elftool: %s: no program header\n", bin->path);
      }
    }
    printf("\n");
  }
}
void elftool_dump_shdr32(t_list *lst_shdr) {
  shdr32_t *shdr = NULL;

  if (lst_shdr) {
    shdr = (shdr32_t *)lst_shdr->content;
  }
  if (shdr) {
    printf("%3d ", shdr->idx);
    // printf("%08x ", shdr->shdr->sh_name);
    printf("%16.16s ", " ");
    if (shdr->bin->shstrtab32 &&
        shdr->bin->shstrtab32->shdr->sh_offset + shdr->shdr->sh_name <
            shdr->bin->length) {
      printf("%16.16s ",
             (char *)&shdr->bin->mem[shdr->bin->shstrtab32->shdr->sh_offset +
                                     shdr->shdr->sh_name]);
    }
    printf("%8x ", shdr->shdr->sh_type);
    printf("%8x ", shdr->shdr->sh_flags);
    printf("%8x ", shdr->shdr->sh_addr);
    printf("%8x ", shdr->shdr->sh_offset);
    printf("%8x ", shdr->shdr->sh_size);
    printf("%4x ", shdr->shdr->sh_link);
    printf("%4x ", shdr->shdr->sh_info);
    printf("%5x ", shdr->shdr->sh_addralign);
    printf("%5x\n", shdr->shdr->sh_entsize);
  }
}

void elftool_dump_shdr64(t_list *lst_shdr) {
  shdr64_t *shdr = NULL;

  if (lst_shdr) {
    shdr = (shdr64_t *)lst_shdr->content;
  }
  if (shdr) {
    printf("%3d ", shdr->idx);
    printf("%16.16s ", " ");
    if (shdr->bin->shstrtab64 &&
        shdr->bin->shstrtab64->shdr->sh_offset + shdr->shdr->sh_name <
            shdr->bin->length) {
      printf("%16.16s ",
             (char *)&shdr->bin->mem[shdr->bin->shstrtab64->shdr->sh_offset +
                                     shdr->shdr->sh_name]);
    }
    printf("%08x ", shdr->shdr->sh_type);
    printf("%10lx ", shdr->shdr->sh_flags);
    printf("%10lx ", shdr->shdr->sh_addr);
    printf("%10lx ", shdr->shdr->sh_offset);
    printf("%10lx ", shdr->shdr->sh_size);
    printf("%4x ", shdr->shdr->sh_link);
    printf("%4x ", shdr->shdr->sh_info);
    printf("%5lx ", shdr->shdr->sh_addralign);
    printf("%5lx\n", shdr->shdr->sh_entsize);
  }
}

void elftool_dump_shdr(elftool_t *bin) {
  printf("===== SECTION HEADER\n");
  if (bin->elfclass == ELFCLASS32) {
    printf("%3s %16s %8s %8s %8s %8s %8s %4.4s %4.4s %5.5s %5.5s\n", "id",
           "name", "type", "flags", "addr", "offset", "size", "link", "info",
           "align", "entsize");
    ft_lstiter(bin->shdr, elftool_dump_shdr32);
  } else {
    printf("%3s %16s %8s %10s %10s %10s %10s %4.4s %4.4s %5.5s %5.5s\n", "id",
           "name", "type", "flags", "addr", "offset", "size", "link", "info",
           "align", "entsize");
    ft_lstiter(bin->shdr, elftool_dump_shdr64);
  }
  printf("\n");
}

void elftool_dump_syms32(t_list *lst_syms) {
  syms32_t *syms = NULL;

  if (lst_syms) {
    syms = (syms32_t *)lst_syms->content;
  }
  if (syms) {
    printf("%3d ", syms->idx);
    if (syms->bin->strtab32->shdr->sh_offset + syms->syms->st_name >
        syms->bin->length) {
      fprintf(stderr, "\nERROR: symbol string out of mem\n");
    } else {
      printf("%32.32s ",
             (char *)&syms->bin->mem[syms->bin->strtab32->shdr->sh_offset +
                                     syms->syms->st_name]);
    }
    printf("%8x ", syms->syms->st_value);
    printf("%6x ", syms->syms->st_size);
    uint8_t bind = ELF32_ST_BIND(syms->syms->st_info);
    uint8_t type = ELF32_ST_TYPE(syms->syms->st_info);
    char buf[8] = {0};
    switch (bind) {
    case 0:
      memcpy(buf, (uint8_t *)"LOCAL", 5);
      break;
    case 1:
      memcpy(buf, (uint8_t *)"GLOBAL", 6);
      break;
    case 2:
      memcpy(buf, (uint8_t *)"WEAK", 4);
      break;
    default:
      memcpy(buf, (uint8_t *)"UNKNW", 5);
      break;
    }
    printf("%8.8s ", buf);
    memset(buf, 0, 8);
    switch (type) {
    case STT_NOTYPE:
      memcpy(buf, (uint8_t *)"NOTYPE", 6);
      break;
    case STT_OBJECT:
      memcpy(buf, (uint8_t *)"OBJ", 3);
      break;
    case STT_FUNC:
      memcpy(buf, (uint8_t *)"FUNC", 4);
      break;
    case STT_SECTION:
      memcpy(buf, (uint8_t *)"SECTION", 7);
      break;
    case STT_FILE:
      memcpy(buf, (uint8_t *)"FILE", 4);
      break;
    case STT_COMMON:
      memcpy(buf, (uint8_t *)"COMMON", 6);
      break;
    case STT_TLS:
      memcpy(buf, (uint8_t *)"TLS", 3);
      break;
    default:
      memcpy(buf, (uint8_t *)"UNKNW", 5);
      break;
    }
    printf("%8.8s ", buf);
    printf("%4hhx ", syms->syms->st_info);
    printf("%4hhd ", syms->syms->st_other);
    printf("%6x\n", syms->syms->st_shndx);
  }
}

void elftool_dump_syms64(t_list *lst_syms) {
  syms64_t *syms = NULL;

  if (lst_syms) {
    syms = (syms64_t *)lst_syms->content;
  }
  if (syms) {
    printf("%3d ", syms->idx);
    //		printf("%16x ", syms->syms->st_name);
    if (syms->bin->strtab64->shdr->sh_offset + syms->syms->st_name >
        syms->bin->length) {
      fprintf(stderr, "\nERROR: symbol string out of mem\n");
    } else {
      printf("%32.32s ",
             (char *)&syms->bin->mem[syms->bin->strtab64->shdr->sh_offset +
                                     syms->syms->st_name]);
    }
    printf("%16lx ", syms->syms->st_value);
    printf("%6lx ", syms->syms->st_size);
    uint8_t bind = ELF32_ST_BIND(syms->syms->st_info);
    uint8_t type = ELF32_ST_TYPE(syms->syms->st_info);
    char buf[8] = {0};
    switch (bind) {
    case 0:
      memcpy(buf, (uint8_t *)"LOCAL", 5);
      break;
    case 1:
      memcpy(buf, (uint8_t *)"GLOBAL", 6);
      break;
    case 2:
      memcpy(buf, (uint8_t *)"WEAK", 4);
      break;
    default:
      memcpy(buf, (uint8_t *)"UNKNW", 5);
      break;
    }
    printf("%8.8s ", buf);
    memset(buf, 0, 8);
    switch (type) {
    case STT_NOTYPE:
      memcpy(buf, (uint8_t *)"NOTYPE", 6);
      break;
    case STT_OBJECT:
      memcpy(buf, (uint8_t *)"OBJ", 3);
      break;
    case STT_FUNC:
      memcpy(buf, (uint8_t *)"FUNC", 4);
      break;
    case STT_SECTION:
      memcpy(buf, (uint8_t *)"SECTION", 7);
      break;
    case STT_FILE:
      memcpy(buf, (uint8_t *)"FILE", 4);
      break;
    case STT_COMMON:
      memcpy(buf, (uint8_t *)"COMMON", 6);
      break;
    case STT_TLS:
      memcpy(buf, (uint8_t *)"TLS", 3);
      break;
    default:
      memcpy(buf, (uint8_t *)"UNKNW", 5);
      break;
    }
    printf("%8.8s ", buf);
    printf("%4hhd ", syms->syms->st_other);
    printf("%6x\n", syms->syms->st_shndx);
  }
}

void elftool_dump_syms(elftool_t *bin) {
  printf("===== SYMBOLS HEADER\n");
  if (bin->elfclass == ELFCLASS32) {
    printf("%3s %32s %8s %6s %8s %8s %4s %6s\n", "id", "name", "value", "size",
           "bind", "type", "other", "shndx");
    ft_lstiter(bin->syms, elftool_dump_syms32);
  } else {
    printf("%3s %32s %16s %6s %8s %8s %4s %6s\n", "id", "name", "value", "size",
           "bind", "type", "other", "shndx");
    ft_lstiter(bin->syms, elftool_dump_syms64);
  }
  printf("\n");
}

void elftool_dump(elftool_opt_t *opt, elftool_t *bin) {

  if (opt->ehdr) {
    elftool_dump_ehdr(bin);
  }
  if (opt->phdr) {
    elftool_dump_phdr(bin);
  }
  if (opt->shdr) {
    elftool_dump_shdr(bin);
  }
  if (opt->syms) {
    elftool_dump_syms(bin);
  }
  if (opt->nm) {
    elftool_dump_nm(bin);
  }
}
