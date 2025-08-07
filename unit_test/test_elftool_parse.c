#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <elf.h>
#include "elftool_parse.h"
#include <string.h>

static void test_elftool_parse64_ehdr(void **state) {
  (void)state;
  Elf64_Ehdr hdr = {
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = 10,
      .e_shoff = 10,
      .e_phentsize = 10,
      .e_phnum = 10,
      .e_shentsize = 10,
      .e_shnum = 10,
  };
  elftool_t bin = {
      .mem = (void *)&hdr,
      .length = 200,
  };
  int r = elftool_parse_ehdr(&bin);
  assert_true(r == 0);
  assert_true(bin.elfclass == ELFCLASS64);
  assert_true(bin.endian == ELFDATA2LSB);
  assert_ptr_equal(bin.ehdr64, &hdr);
}

static void test_elftool_parse64_phdr(void **state) {
  (void)state;
  char buf[200];
  memset(buf, 0, 200);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 200,
  };
  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = sizeof(Elf64_Ehdr),
      .e_shoff = 0,
      .e_phentsize = sizeof(Elf64_Phdr),
      .e_phnum = 1,
      .e_shentsize = 0,
      .e_shnum = 0,
  };
  Elf64_Phdr *phdr = (Elf64_Phdr *)(buf + sizeof(Elf64_Ehdr));
  *phdr = (Elf64_Phdr){
      .p_type = PT_LOAD,
  };

  int r = elftool_parse_ehdr(&bin);
  assert_int_equal(r, 0);
  r = elftool_parse_phdr(&bin);
  assert_int_equal(r, 0);
  assert_non_null(bin.phdr);
}

static void test_elftool_parse64_shdr(void **state) {
  (void)state;
  char buf[300];
  memset(buf, 0, 300);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 300,
  };
  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = 0,
      .e_shoff = sizeof(Elf64_Ehdr),
      .e_phentsize = 0,
      .e_phnum = 0,
      .e_shentsize = sizeof(Elf64_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf64_Shdr *shdr_tbl = (Elf64_Shdr *)(buf + sizeof(Elf64_Ehdr));
  // SYMTAB
  shdr_tbl[0] = (Elf64_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
  };
  // STRTAB for shdr names
  shdr_tbl[1] = (Elf64_Shdr){
      .sh_type = SHT_STRTAB,
  };

  int r = elftool_parse_ehdr(&bin);
  assert_int_equal(r, 0);
  r = elftool_parse_shdr(&bin);
  assert_int_equal(r, 0);
  assert_non_null(bin.shdr);
}

static void test_elftool_parse64_syms(void **state) {
  (void)state;
  char buf[500];
  memset(buf, 0, 500);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 500,
  };
  Elf64_Ehdr *hdr = (Elf64_Ehdr *)buf;
  *hdr = (Elf64_Ehdr){
      .e_ident = {0x7f, 0x45, 0x4c, 0x46, ELFCLASS64, ELFDATA2LSB, 0x11},
      .e_phoff = 0,
      .e_shoff = sizeof(Elf64_Ehdr),
      .e_phentsize = 0,
      .e_phnum = 0,
      .e_shentsize = sizeof(Elf64_Shdr),
      .e_shnum = 2,
      .e_shstrndx = 1,
  };
  Elf64_Shdr *shdr_tbl = (Elf64_Shdr *)(buf + sizeof(Elf64_Ehdr));
  // SYMTAB
  shdr_tbl[0] = (Elf64_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Shdr) * 2,
      .sh_size = sizeof(Elf64_Sym),
      .sh_entsize = sizeof(Elf64_Sym),
  };
  // STRTAB for shdr names
  shdr_tbl[1] = (Elf64_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf64_Sym *sym = (Elf64_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf64_Sym){
      .st_name = 0,
  };

  int r = elftool_parse_ehdr(&bin);
  assert_int_equal(r, 0);
  r = elftool_parse_shdr(&bin);
  assert_int_equal(r, 0);
  r = elftool_parse_syms(&bin);
  assert_int_equal(r, 0);
  assert_non_null(bin.syms);
}

static void test_elftool_parse32_all(void **state) {
  (void)state;
  char buf[500];
  memset(buf, 0, 500);
  elftool_t bin = {
      .mem = (void *)buf,
      .length = 500,
  };
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
  // SYMTAB
  shdr_tbl[0] = (Elf32_Shdr){
      .sh_type = SHT_SYMTAB,
      .sh_link = 1,
      .sh_offset = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) + sizeof(Elf32_Shdr) * 2,
      .sh_size = sizeof(Elf32_Sym),
      .sh_entsize = sizeof(Elf32_Sym),
  };
  // STRTAB for shdr names
  shdr_tbl[1] = (Elf32_Shdr){
      .sh_type = SHT_STRTAB,
  };
  Elf32_Sym *sym = (Elf32_Sym *)(buf + shdr_tbl[0].sh_offset);
  *sym = (Elf32_Sym){
      .st_name = 0,
  };

  int r = elftool_parse(&bin);
  assert_int_equal(r, 0);
  assert_non_null(bin.phdr);
  assert_non_null(bin.shdr);
  assert_non_null(bin.syms);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_elftool_parse64_ehdr),
      cmocka_unit_test(test_elftool_parse64_phdr),
      cmocka_unit_test(test_elftool_parse64_shdr),
      cmocka_unit_test(test_elftool_parse64_syms),
      cmocka_unit_test(test_elftool_parse32_all),
  };
  cmocka_set_message_output(CM_OUTPUT_XML);
  return cmocka_run_group_tests(tests, NULL, NULL);
}
