#include "elftool.h"

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
    for (list_t *head = bin->phdr; head; head = head->next) {
      if (head->content) {
        lseek(fd, get_phdr64_ent(head->content)->p_offset, SEEK_SET);
        write(fd, get_phdr64_mem(head->content), get_phdr64_ent(head->content)->p_filesz);
      }
    }
    //for (list_t *head = bin->shdr; head; head = head->next) {
    //  if (head->content) {
    //    lseek(fd, ((shdr64_t *)head->content)->shdr->sh_offset, SEEK_SET);
    //    write(fd, ((shdr64_t *)head->content)->mem,
    //          ((shdr64_t *)head->content)->shdr->sh_size);
    //  }
    //}
    //lseek(fd, get_ehdr64(bin)->e_shoff, SEEK_SET);
    //for (list_t *head = bin->shdr; head; head = head->next) {
    //  if (head->content) {
    //    write(fd, ((shdr64_t *)head->content)->shdr, get_ehdr64(bin)->e_shentsize);
    //  }
    //}
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
    for (list_t *head = bin->phdr; head; head = head->next) {
      if (head->content) {
        lseek(fd, get_phdr32_ent(head->content)->p_offset, SEEK_SET);
        write(fd, get_phdr32_mem(head->content), get_phdr32_ent(head->content)->p_filesz);
      }
    }
    // for (list_t *head = bin->shdr; head; head = head->next) {
    //   if (head->content) {
    //     lseek(fd, ((shdr32_t *)head->content)->shdr->sh_offset, SEEK_SET);
    //     write(fd, ((shdr32_t *)head->content)->mem,
    //           ((shdr32_t *)head->content)->shdr->sh_size);
    //   }
    // }
    // lseek(fd, bin->ehdr32->e_shoff, SEEK_SET);
    // for (list_t *head = bin->shdr; head; head = head->next) {
    //   if (head->content) {
    //     write(fd, ((shdr32_t *)head->content)->shdr, bin->ehdr32->e_shentsize);
    //   }
    // }
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
