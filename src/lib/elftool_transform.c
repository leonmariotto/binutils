#include "elftool_transform.h"
#include "elftool.h"
#include <string.h>

// TODO LMA: should create a new PT_LOAD with section injection.

int elftool_transform(elftool_t *bin, elftool_transform_t *transform) {
  int r = 0;

  if (!bin || !transform) {
    r = -1;
  } else {
    if (transform->type == NM_CODECAVE_INJECT) {
      r = elftool_transform_codecave_injection(bin, transform);
    } else if (transform->type == NM_SILVIO_INJECT) {
      r = elftool_transform_silvio_injection(bin, transform);
    } else {
      r = elftool_transform_section_injection(bin, transform);
    }
  }
  return (r);
}

int elftool_transform_codecave_injection(elftool_t *bin,
                                        elftool_transform_t *transform) {
  int r = 0;

  if (!bin || !transform) {
    r = -1;
  } else {
    r = 0;
  }
  return (r);
}

/*
 * @brief extract info from last PT_LOAD
 *
 * vaddr is filled with the next virtual addr available in binary (at runtime)
 * after .bss section.
 * fileoff is filled with the next available offset for writting section content.
 * TODO: fileoff is unecessary, only need to append data to file (fileoff == file size)
 *
 * @return  0 on succes
 *          -1 on error
 * */
int get_vaddr(elftool_t *bin, uint64_t *vaddr)
{
  phdr64_t *last_ptload = NULL;
  int r = 0;

  for (list_t *head = bin->phdr; head; head = head->next) {
    if (((phdr64_t *)head->content)->phdr->p_type == PT_LOAD &&
        (!last_ptload || last_ptload->phdr->p_vaddr <
                             ((phdr64_t *)head->content)->phdr->p_vaddr)) {
      last_ptload = (phdr64_t *)head->content;
    }
  }
  if (!last_ptload) {
      r = -1;
  }
  else {
      *vaddr = last_ptload->phdr->p_offset + last_ptload->phdr->p_memsz;
  }
  return (r);
}

int inject_shdr64(elftool_t *bin, elftool_transform_t *transform)
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
  for (list_t *head = bin->phdr; head; head = head->next) {
    if (((phdr64_t *)head->content)->phdr->p_type == PT_LOAD &&
        (!last_ptload || last_ptload->phdr->p_vaddr <
                             ((phdr64_t *)head->content)->phdr->p_vaddr)) {
      last_ptload = (phdr64_t *)head->content;
    }
  }
  if (!last_ptload) {
    r = -1;
  } else {
    fprintf(stderr, "%s:%d\n", __func__, __LINE__);
    /* Find last section for this segment */
    for (list_t *head = bin->shdr; head; head = head->next) {
      if (((shdr64_t *)head->content)->shdr->sh_offset >
              last_ptload->phdr->p_offset &&
          ((shdr64_t *)head->content)->shdr->sh_offset <
              last_ptload->phdr->p_offset + last_ptload->phdr->p_filesz &&
          (!last_shdr || last_shdr->shdr->sh_offset <
                             ((shdr64_t *)head->content)->shdr->sh_offset)) {
        last_shdr = (shdr64_t *)head->content;
      }
    }
    if (!last_shdr) {
      r = -1;
    }
    if (r == 0) {
      fprintf(stderr, "%s:%d\n", __func__, __LINE__);
      /* Build new shdr */
      new_shdr = malloc(bin->ehdr64->e_shentsize);
      if (!new_shdr) {
        r = -1;
      }
    }
    if (r == 0) {
      fprintf(stderr, "%s:%d\n", __func__, __LINE__);
      memset(new_shdr, 0, bin->ehdr64->e_shentsize);
      new_shdr->sh_type = SHT_PROGBITS;
      new_shdr->sh_flags = SHF_EXECINSTR;
      new_shdr->sh_addr = last_shdr->shdr->sh_offset + last_shdr->shdr->sh_size;
      new_shdr->sh_offset =
          last_shdr->shdr->sh_offset + last_shdr->shdr->sh_size;
      new_shdr->sh_size = transform->code_len;
      new_shdr->sh_link = 0;
      new_shdr->sh_info = 0;
      new_shdr->sh_addralign = 8;
      new_shdr->sh_entsize = 0;
      new_shdr_entry.shdr = new_shdr;
      new_shdr_entry.mem = transform->code;
      new_shdr_entry.idx = last_shdr->idx + 1;
      fprintf(stderr, "%s:%d\n", __func__, __LINE__);
      for (list_t *head = bin->shdr; head && r == 0; head = head->next) {
        /* Increase next idx */
        if (head->content &&
            ((shdr64_t *)head->content)->idx > last_shdr->idx) {
          ((shdr64_t *)head->content)->idx += 1;
        }
        /* Update offset */
        if (head->content && ((shdr64_t *)head->content)->shdr->sh_offset >
                                 new_shdr->sh_offset) {
          fprintf(stderr, "%s:%d\n", __func__, __LINE__);
          ((shdr64_t *)head->content)->shdr->sh_offset += code_len_aligned;
          ((shdr64_t *)head->content)->shdr->sh_addr += code_len_aligned;
        }
      }
      fprintf(stderr, "%s:%d\n", __func__, __LINE__);
      /* Add the new section in the section list */
      for (list_t *head = bin->shdr; head && r == 0; head = head->next) {
        if (((shdr64_t *)head->content)->idx == last_shdr->idx) {
          list_t *new = ft_lstnew(&new_shdr_entry, sizeof(new_shdr_entry));
          if (!new) {
            r = -1;
          } else {
            new->next = head->next;
            head->next = new;
          }
        }
        if (((shdr64_t *)head->content)->shdr->sh_type == SHT_DYNSYM ||
            ((shdr64_t *)head->content)->shdr->sh_type == SHT_SYMTAB) {
          if (((shdr64_t *)head->content)->shdr->sh_link > new_shdr_entry.idx) {
            ((shdr64_t *)head->content)->shdr->sh_link += 1;
          }
        }
      }
    }
    if (r == 0) {
      fprintf(stderr, "%s:%d\n", __func__, __LINE__);
      /* Update size and offset in phdr table */
      last_ptload->phdr->p_filesz += code_len_aligned;
      last_ptload->phdr->p_memsz += code_len_aligned;
      for (list_t *head = bin->phdr; head; head = head->next) {
        if (((phdr64_t *)head->content)->phdr->p_offset >
            last_ptload->phdr->p_offset +
                (last_ptload->phdr->p_filesz - code_len_aligned)) {
          ((phdr64_t *)head->content)->phdr->p_offset += code_len_aligned;
          ((phdr64_t *)head->content)->phdr->p_vaddr += code_len_aligned;
        }
      }
    }
    if (r == 0) {
      if (bin->ehdr64->e_shstrndx >= new_shdr_entry.idx) {
        bin->ehdr64->e_shstrndx += 1;
      }
      bin->ehdr64->e_shnum += 1;
    }
  }
  return (r);
}

/* @brief Append the code in @param transform to binary.
 * Fill transform->file_offset with code file offset.
 * */
int elftool_append_code(elftool_t *bin, elftool_transform_t *transform)
{
    uint8_t *new_mem;

    if (!bin || !transform)
        return EINVAL;
    new_mem = malloc(bin->length + transform->code_len);
    if (new_mem == NULL)
        return ENOMEM;
    memcpy(new_mem, bin->mem, bin->length);
    memcpy(&new_mem[bin->length], transform->code, transform->code_len);
    transform->code_file_offset = bin->length;
    bin->length = bin->length + transform->code_len;
    free(bin->mem);
    bin->mem = new_mem;
    return 0;
}

/* @brief Add a new phdr
 * Relocate the whole phdr table at the end of the file.
 * Fill transform->phdr_file_offset.
 * */
int elftool_add_phdr(elftool_t *bin, elftool_transform_t *transform)
{
    uint8_t *new_mem;

    if (!bin || !transform)
        return EINVAL;
    Elf64_Off phdr_table_size = bin->ehdr64->e_phoff +
        (((Elf64_Off)bin->ehdr64->e_phnum) * (Elf64_Off)bin->ehdr64->e_phentsize);
    new_mem = malloc(bin->length + phdr_table_size + (Elf64_Off)bin->ehdr64->e_phentsize);
    if (new_mem == NULL)
        return ENOMEM;
    memcpy(new_mem, bin->mem, bin->length);
    memcpy(&new_mem[bin->length], &bin->mem[bin->ehdr64->e_phoff], phdr_table_size);
    memset(&new_mem[bin->length + phdr_table_size], 0x42, sizeof(Elf64_Phdr));
    bin->ehdr64->e_phoff = bin->length; // Relocate program header offset
    bin->ehdr64->e_phnum += 1;
    transform->phdr_file_offset = bin->length + phdr_table_size;
    bin->length += phdr_table_size + (size_t)bin->ehdr64->e_phentsize;
    free(bin->mem);
    bin->mem = new_mem;
    return 0;
}

int elftool_transform_section_injection(elftool_t *bin,
                                       elftool_transform_t *transform) {
  int r = 0;
  Elf64_Addr vaddr = 0;

  if (!bin || !transform) {
    r = -1;
  } else {
    if (bin->elfclass == ELFCLASS32) {
      r = ENOSYS;
    } else {
      fprintf(stderr, "%s:%d\n", __func__, __LINE__);
      r = elftool_append_code(bin, transform);
      if (r == 0) {
        r = elftool_add_phdr(bin, transform);
      }
      if (r == 0) {
        r = get_vaddr(bin, &vaddr);
      }
      if (r == 0) {
          phdr64_t new_phdr = {
              .idx = bin->ehdr64->e_phnum - 1,
              .phdr = (Elf64_Phdr *)&bin->mem[transform->phdr_file_offset],
              .bin = bin,
          };
          new_phdr.phdr->p_type = PT_LOAD;			/* Segment type */
          new_phdr.phdr->p_flags = PF_R | PF_W;		/* Segment flags */
          new_phdr.phdr->p_offset = transform->code_file_offset;		/* Segment file offset */
          new_phdr.phdr->p_vaddr = vaddr;		/* Segment virtual address */
          new_phdr.phdr->p_paddr = 0;		/* Segment physical address */
          new_phdr.phdr->p_filesz = transform->code_len;		/* Segment size in file */
          new_phdr.phdr->p_memsz = transform->code_len;		/* Segment size in memory */
          new_phdr.phdr->p_align = 0x1000;		/* Segment alignment */
          list_t *new = ft_lstnew(&new_phdr, sizeof(new_phdr));
          ft_lstpush(&bin->phdr, new);
      }
    }
  }
  return (r);
}

int elftool_transform_silvio_injection(elftool_t *bin,
                                      elftool_transform_t *transform) {
  int r = 0;

  if (!bin || !transform) {
    r = -1;
  } else {
    r = 0;
  }
  return (r);
}
