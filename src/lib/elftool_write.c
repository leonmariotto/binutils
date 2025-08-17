#include "elftool.h"

#include <string.h>

// TODO copy symbols table and section table
void elftool_write64(elftool_t *bin, int fd) {
  if (bin) {
    write(fd, bin->mem, 0x40); // Copy ELF header.
    /* Copy phdr table */
    lseek(fd, get_ehdr64(bin)->e_phoff, SEEK_SET);
    for (list_t *head = bin->phdr; head; head = head->next) {
      if (head->content) {
        write(fd, get_phdr64_ent(head->content), get_ehdr64(bin)->e_phentsize);
      }
    }
    /* Copy section table: optional */
    lseek(fd, get_ehdr64(bin)->e_shoff, SEEK_SET);
    for (list_t *head = bin->shdr; head; head = head->next) {
      if (head->content) {
        write(fd, get_shdr64_ent(head->content), get_ehdr64(bin)->e_shentsize);
      }
    }
    /* Copy sections content: optional */
    for (list_t *head = bin->shdr; head; head = head->next) {
      if (head->content) {
        lseek(fd, get_shdr64_ent(head->content)->sh_offset, SEEK_SET);
        write(fd, get_shdr64_mem(head->content),
              get_shdr64_ent(head->content)->sh_size);
      }
    }
    /* Copy phdr content */
    for (list_t *head = bin->phdr; head; head = head->next) {
      if (head->content) {
        lseek(fd, get_phdr64_ent(head->content)->p_offset, SEEK_SET);
        write(fd, get_phdr64_mem(head->content), get_phdr64_ent(head->content)->p_filesz);
      }
    }
  }
}
void elftool_write32(elftool_t *bin, int fd) {
  if (bin) {
    write(fd, bin->mem, 0x40); // Copy ELF header.
    /* Copy phdr table */
    lseek(fd, get_ehdr32(bin)->e_phoff, SEEK_SET);
    for (list_t *head = bin->phdr; head; head = head->next) {
      if (head->content) {
        write(fd, get_phdr32_ent(head->content), get_ehdr32(bin)->e_phentsize);
      }
    }
    /* Copy section table: optional */
    lseek(fd, get_ehdr32(bin)->e_shoff, SEEK_SET);
    for (list_t *head = bin->shdr; head; head = head->next) {
      if (head->content) {
        write(fd, get_shdr32_ent(head->content), get_ehdr32(bin)->e_shentsize);
      }
    }
    /* Copy sections content: optional */
    for (list_t *head = bin->shdr; head; head = head->next) {
      if (head->content) {
        lseek(fd, get_shdr32_ent(head->content)->sh_offset, SEEK_SET);
        write(fd, get_shdr32_mem(head->content),
              get_shdr32_ent(head->content)->sh_size);
      }
    }
    /* Copy phdr content */
    for (list_t *head = bin->phdr; head; head = head->next) {
      if (head->content) {
        lseek(fd, get_phdr32_ent(head->content)->p_offset, SEEK_SET);
        write(fd, get_phdr32_mem(head->content), get_phdr32_ent(head->content)->p_filesz);
      }
    }
}

int elftool_write(elftool_t *bin, char *file_out) {
  int r = 0;
  int fd = 0;

  if (!bin || !file_out) {
    r = -1;
  } else {
    fd = open(file_out, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd < 0) {
      r = -1;
    } else {
      void *buf = malloc(bin->length);
      if (!buf) {
          close(fd);
          return ENOMEM;
      }
      memset(buf, 0, bin->length);
      write(fd, buf, bin->length);
      free(buf);
      if (bin->elfclass == ELFCLASS32) {
        elftool_write32(bin, fd);
      } else {
        elftool_write64(bin, fd);
      }
      close(fd);
    }
  }
  return (r);
}
