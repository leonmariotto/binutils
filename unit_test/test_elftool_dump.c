#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <elf.h>
#include "elftool.h"
#include "elftool_dump.h"
#include "elftool_parse.h"

static void test_elftool_dump_all(void **state) {
  (void)state;
  char buf[1024];
  memset(buf, 0, 1024);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 1024,
  };

  // Create a 64-bit ELF file with all headers
  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = sizeof(Elf64_Ehdr),
      .e_shoff = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
      .e_phentsize = sizeof(Elf64_Phdr),
      .e_phnum = 1,
      .e_shentsize = sizeof(Elf64_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf64_Phdr *phdr = (Elf64_Phdr *)(buf + sizeof(Elf64_Ehdr));
  *phdr = (Elf64_Phdr){
      .p_type = PT_LOAD,
  };
  Elf64_Shdr *shdr_tbl = (Elf64_Shdr *)(buf + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr));
  shdr_tbl[0] = (Elf64_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr) * 2,
      .sh_size = sizeof(Elf64_Sym),
      .sh_entsize = sizeof(Elf64_Sym),
  };
  shdr_tbl[1] = (Elf64_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf64_Sym *sym = (Elf64_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf64_Sym){
      .st_name = 0,
  };

  assert_int_equal(elftool_parse(&bin), 0);

  elftool_opt_t opt = {
    .ehdr = true,
    .phdr = true,
    .shdr = true,
    .syms = true,
    .nm = true,
  };
  elftool_dump(&opt, &bin);
}

static void test_elftool_dump_all_32(void **state) {
  (void)state;
  char buf[1024];
  memset(buf, 0, 1024);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 1024,
  };

  // Create a 32-bit ELF file with all headers
  Elf32_Ehdr *hdr = (Elf32_Ehdr *)buf;
  *hdr = (Elf32_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS32, ELFDATA2LSB, 0x11},
      .e_phoff = sizeof(Elf32_Ehdr),
      .e_shoff = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr),
      .e_phentsize = sizeof(Elf32_Phdr),
      .e_phnum = 1,
      .e_shentsize = sizeof(Elf32_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf32_Phdr *phdr = (Elf32_Phdr *)(buf + sizeof(Elf32_Ehdr));
  *phdr = (Elf32_Phdr){
      .p_type = PT_LOAD,
  };
  Elf32_Shdr *shdr_tbl = (Elf32_Shdr *)(buf + sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr));
  shdr_tbl[0] = (Elf32_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) + sizeof(Elf32_Shdr) * 2,
      .sh_size = sizeof(Elf32_Sym),
      .sh_entsize = sizeof(Elf32_Sym),
  };
  shdr_tbl[1] = (Elf32_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf32_Sym *sym = (Elf32_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf32_Sym){
      .st_name = 0,
  };

  assert_int_equal(elftool_parse(&bin), 0);

  elftool_opt_t opt = {
    .ehdr = true,
    .phdr = true,
    .shdr = true,
    .syms = true,
    .nm = true,
  };
  elftool_dump(&opt, &bin);
}

static void test_elftool_dump_weak_object(void **state) {
  (void)state;
  char buf[1024];
  memset(buf, 0, 1024);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 1024,
  };

  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = sizeof(Elf64_Ehdr),
      .e_shoff = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
      .e_phentsize = sizeof(Elf64_Phdr),
      .e_phnum = 1,
      .e_shentsize = sizeof(Elf64_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf64_Phdr *phdr = (Elf64_Phdr *)(buf + sizeof(Elf64_Ehdr));
  *phdr = (Elf64_Phdr){
      .p_type = PT_LOAD,
  };
  Elf64_Shdr *shdr_tbl = (Elf64_Shdr *)(buf + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr));
  shdr_tbl[0] = (Elf64_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr) * 2,
      .sh_size = sizeof(Elf64_Sym),
      .sh_entsize = sizeof(Elf64_Sym),
  };
  shdr_tbl[1] = (Elf64_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf64_Sym *sym = (Elf64_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf64_Sym){
      .st_name = 0,
      .st_info = ELF64_ST_INFO(STB_WEAK, STT_OBJECT),
  };

  assert_int_equal(elftool_parse(&bin), 0);

  elftool_opt_t opt = {
    .nm = true,
  };
  elftool_dump(&opt, &bin);
}

static void test_elftool_dump_shn_abs(void **state) {
  (void)state;
  char buf[1024];
  memset(buf, 0, 1024);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 1024,
  };

  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = sizeof(Elf64_Ehdr),
      .e_shoff = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
      .e_phentsize = sizeof(Elf64_Phdr),
      .e_phnum = 1,
      .e_shentsize = sizeof(Elf64_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf64_Phdr *phdr = (Elf64_Phdr *)(buf + sizeof(Elf64_Ehdr));
  *phdr = (Elf64_Phdr){
      .p_type = PT_LOAD,
  };
  Elf64_Shdr *shdr_tbl = (Elf64_Shdr *)(buf + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr));
  shdr_tbl[0] = (Elf64_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr) * 2,
      .sh_size = sizeof(Elf64_Sym),
      .sh_entsize = sizeof(Elf64_Sym),
  };
  shdr_tbl[1] = (Elf64_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf64_Sym *sym = (Elf64_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf64_Sym){
      .st_name = 0,
      .st_shndx = SHN_ABS,
  };

  assert_int_equal(elftool_parse(&bin), 0);

  elftool_opt_t opt = {
    .nm = true,
  };
  elftool_dump(&opt, &bin);
}

static void test_elftool_dump_stt_gnu_ifunc(void **state) {
  (void)state;
  char buf[1024];
  memset(buf, 0, 1024);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 1024,
  };

  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = sizeof(Elf64_Ehdr),
      .e_shoff = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
      .e_phentsize = sizeof(Elf64_Phdr),
      .e_phnum = 1,
      .e_shentsize = sizeof(Elf64_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf64_Phdr *phdr = (Elf64_Phdr *)(buf + sizeof(Elf64_Ehdr));
  *phdr = (Elf64_Phdr){
      .p_type = PT_LOAD,
  };
  Elf64_Shdr *shdr_tbl = (Elf64_Shdr *)(buf + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr));
  shdr_tbl[0] = (Elf64_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(Elf64_Shdr) * 2,
      .sh_size = sizeof(Elf64_Sym),
      .sh_entsize = sizeof(Elf64_Sym),
  };
  shdr_tbl[1] = (Elf64_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf64_Sym *sym = (Elf64_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf64_Sym){
      .st_name = 0,
      .st_info = ELF64_ST_INFO(STB_GLOBAL, STT_GNU_IFUNC),
  };

  assert_int_equal(elftool_parse(&bin), 0);

  elftool_opt_t opt = {
    .nm = true,
  };
  elftool_dump(&opt, &bin);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_elftool_dump_all),
      cmocka_unit_test(test_elftool_dump_all_32),
      cmocka_unit_test(test_elftool_dump_weak_object),
      cmocka_unit_test(test_elftool_dump_shn_abs),
      cmocka_unit_test(test_elftool_dump_stt_gnu_ifunc),
  };
  cmocka_set_message_output(CM_OUTPUT_XML);
  return cmocka_run_group_tests(tests, NULL, NULL);
}
