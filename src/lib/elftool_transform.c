#include "elftool_transform.h"
#include "elftool.h"
#include <string.h>

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
    if (get_phdr64_ent(head->content)->p_type == PT_LOAD &&
        (!last_ptload || get_phdr64_ent(last_ptload)->p_vaddr <
                             get_phdr64_ent(head->content)->p_vaddr)) {
      last_ptload = (phdr64_t *)head->content;
    }
  }
  if (!last_ptload) {
      r = -1;
  }
  else {
      *vaddr = get_phdr64_ent(last_ptload)->p_vaddr + get_phdr64_ent(last_ptload)->p_memsz;
      /* Align to 0x1000 */
      *vaddr = ((((*vaddr) + (0x1000 - 1)) >> 12) << 12);
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
    size_t offset = bin->length;
    offset = ((((offset) + (0x1000 - 1)) >> 12) << 12);
    new_mem = malloc(offset + transform->code_len);
    if (new_mem == NULL)
        return ENOMEM;
    memcpy(new_mem, bin->mem, bin->length);
    // offset aligned to page size (0x1000)
    memcpy(&new_mem[offset], transform->code, transform->code_len);
    printf("Append new data at offset %zu\n", offset);
    transform->code_file_offset = offset;
    bin->length = offset + transform->code_len;
    free(bin->mem);
    bin->mem = new_mem;
    return 0;
}

/* @brief Override first PT_NOTE phdr
 * This allow us to add a new section without moving the whole phdr table.
 * */
int elftool_get_available_phdr_offset(elftool_t *bin, elftool_transform_t *transform)
{
    if (!bin || !transform)
        return EINVAL;
    for (list_t *head = bin->phdr; head; head = head->next) {
        if (get_phdr64_ent(head->content)->p_type == PT_NOTE) {
            transform->phdr_file_offset = ((phdr64_t*)head->content)->p_offset;
            return 0;
        }
    }
    return EINVAL;
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
    Elf64_Off phdr_table_size = get_ehdr64(bin)->e_phoff +
        (((Elf64_Off)get_ehdr64(bin)->e_phnum) * (Elf64_Off)get_ehdr64(bin)->e_phentsize);
    new_mem = malloc(bin->length + phdr_table_size + (Elf64_Off)get_ehdr64(bin)->e_phentsize);
    if (new_mem == NULL)
        return ENOMEM;
    memcpy(new_mem, bin->mem, bin->length);
    memcpy(&new_mem[bin->length], &bin->mem[get_ehdr64(bin)->e_phoff], phdr_table_size);
    memset(&new_mem[bin->length + phdr_table_size], 0x0, sizeof(Elf64_Phdr));
    transform->phdr_file_offset = bin->length + phdr_table_size;
    bin->length += phdr_table_size + (size_t)get_ehdr64(bin)->e_phentsize;
    free(bin->mem);
    bin->mem = new_mem;
    get_ehdr64(bin)->e_phoff = bin->length; // Relocate program header offset
    get_ehdr64(bin)->e_phnum += 1;
    return 0;
}

int elftool_transform_segment_injection(elftool_t *bin,
                                       elftool_transform_t *transform) {
  int r = 0;
  Elf64_Addr vaddr = 0;

  if (!bin || !transform) {
    r = -1;
  } else {
    if (bin->elfclass == ELFCLASS32) {
      r = ENOSYS;
    } else {
      r = elftool_append_code(bin, transform);
      if (r == 0) {
        r = elftool_get_available_phdr_offset(bin, transform);
        // r = elftool_add_phdr(bin, transform);
      }
      if (r == 0) {
        r = get_vaddr(bin, &vaddr);
        transform->virtual_addr = vaddr;
      }
      if (r == 0) {
          phdr64_t new_phdr = {
              .idx = ((Elf64_Ehdr*)bin->mem)->e_phnum - 1,
              .p_offset = transform->phdr_file_offset,
              .mem_offset = transform->code_file_offset,
              .bin = bin,
          };
          get_phdr64_ent(&new_phdr)->p_type = PT_LOAD;
          get_phdr64_ent(&new_phdr)->p_flags = PF_R | PF_W;
          get_phdr64_ent(&new_phdr)->p_offset = transform->code_file_offset;
          get_phdr64_ent(&new_phdr)->p_vaddr = vaddr;
          get_phdr64_ent(&new_phdr)->p_paddr = 0;
          get_phdr64_ent(&new_phdr)->p_filesz = transform->code_len;
          get_phdr64_ent(&new_phdr)->p_memsz = transform->code_len;
          get_phdr64_ent(&new_phdr)->p_align = 0x1000;
          list_t *new = ft_lstnew(&new_phdr, sizeof(new_phdr));
          ft_lstpush(&bin->phdr, new);
      }
    }
  }
  return (r);
}
