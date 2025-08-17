#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "elftool.h"
#include "elftool_query.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOCK_VADDR 0x400000
#define MOCK_SYMBOL_NAME "my_symbol"
#define MOCK_SYMBOL_CONTENT "symbol_content"
#define MOCK_SYMBOL_SIZE (sizeof(MOCK_SYMBOL_CONTENT))

static void setup_elftool(elftool_t *bin) {
    memset(bin, 0, sizeof(elftool_t));
    bin->elfclass = ELFCLASS64;
    bin->mem = (uint8_t*)malloc(1024);
    assert_non_null(bin->mem);
    memset(bin->mem, 0, 1024);
    bin->length = 1024;

    // ELF Header
    Elf64_Ehdr *ehdr = get_ehdr64(bin);
    ehdr->e_ident[EI_CLASS] = ELFCLASS64;
    ehdr->e_phoff = sizeof(Elf64_Ehdr);
    ehdr->e_phnum = 1;
    ehdr->e_phentsize = sizeof(Elf64_Phdr);

    // Program Header
    Elf64_Phdr *phdr = (Elf64_Phdr *)(bin->mem + ehdr->e_phoff);
    phdr->p_type = PT_LOAD;
    phdr->p_vaddr = MOCK_VADDR;
    phdr->p_offset = 0x100;
    phdr->p_filesz = 0x100;
    phdr->p_memsz = 0x100;

    // phdr list
    phdr64_t phdr_wrapper;
    memset(&phdr_wrapper, 0, sizeof(phdr64_t));
    phdr_wrapper.p_offset = ehdr->e_phoff;
    phdr_wrapper.mem_offset = ehdr->e_phoff;
    phdr_wrapper.bin = bin;
    ft_lstadd(&bin->phdr, ft_lstnew(&phdr_wrapper, sizeof(phdr_wrapper)));

    // Symbol
    Elf64_Sym sym;
    sym.st_name = 0; // The name is a pointer in the string table, not used here
    sym.st_value = MOCK_VADDR + 0x10;
    sym.st_size = MOCK_SYMBOL_SIZE;

    // Symbol list
    sym64_t sym_wrapper;
    memset(&sym_wrapper, 0, sizeof(sym64_t));
    sym_wrapper.name = MOCK_SYMBOL_NAME;
    sym_wrapper.bin = bin;
    // The test will not use the real sym table, so we store the sym in the wrapper
    memcpy(bin->mem + phdr->p_offset + (sym.st_value - phdr->p_vaddr), MOCK_SYMBOL_CONTENT, MOCK_SYMBOL_SIZE);
    sym_wrapper.sym_offset = 0; // Not used
    ft_lstadd(&bin->syms, ft_lstnew(&sym_wrapper, sizeof(sym_wrapper)));

    // Hack to store the symbol in the wrapper for the test
    // In a real scenario, the sym would be in the .symtab section
    // and get_sym64_ent would be used to retrieve it.
    // For this test, we will mock get_sym64_ent to return a pointer to our sym.
    // This is a limitation of the current test framework.
    // We will replace the content of get_sym64_ent with a mock function.
}

// Mocking get_sym64_ent is complex, so we will cheat and put the symbol in the list
// and then create a mock get_sym64_ent that returns it.
// For now, let's assume the logic inside elftool_sym_query_by_name can be tested
// without a full mock of get_sym64_ent. The current implementation uses it.

static void test_query_read(void **state) {
    (void)state;
    elftool_t bin;
    setup_elftool(&bin);

    // This is a hack because get_sym64_ent is a macro
    // We will put the symbol at a known offset in memory
    // and make the sym_wrapper point to it.
    sym64_t *sym_wrapper = (sym64_t*)bin.syms->content;
    Elf64_Sym *sym = (Elf64_Sym*)(bin.mem + 0x200);
    sym->st_value = MOCK_VADDR + 0x10;
    sym->st_size = MOCK_SYMBOL_SIZE;
    sym_wrapper->sym_offset = 0x200;


    elftool_sym_query_t query = {0};
    query.op = ELFTOOL_QUERY_READ;

    int r = elftool_sym_query_by_name(&bin, &query, MOCK_SYMBOL_NAME);
    assert_int_equal(r, 0);
    assert_int_equal(query.sym_size, MOCK_SYMBOL_SIZE);
    assert_memory_equal(query.sym_content, MOCK_SYMBOL_CONTENT, MOCK_SYMBOL_SIZE);
    assert_int_equal(query.sym_vaddr, MOCK_VADDR + 0x10);

    free(bin.mem);
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.syms, default_del);
}

static void test_query_write(void **state) {
    (void)state;
    elftool_t bin;
    setup_elftool(&bin);

    sym64_t *sym_wrapper = (sym64_t*)bin.syms->content;
    Elf64_Sym *sym = (Elf64_Sym*)(bin.mem + 0x200);
    sym->st_value = MOCK_VADDR + 0x10;
    sym->st_size = MOCK_SYMBOL_SIZE;
    sym_wrapper->sym_offset = 0x200;

    elftool_sym_query_t query = {0};
    query.op = ELFTOOL_QUERY_WRITE;
    char *new_content = "new_content\0\0\0\0";
    query.sym_content = (uint8_t*)new_content;
    query.sym_size = MOCK_SYMBOL_SIZE;

    int r = elftool_sym_query_by_name(&bin, &query, MOCK_SYMBOL_NAME);
    assert_int_equal(r, 0);

    // Verify the write
    char *symbol_in_mem = (char*)(bin.mem + 0x100 + 0x10);
    assert_memory_equal(symbol_in_mem, new_content, MOCK_SYMBOL_SIZE);

    free(bin.mem);
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.syms, default_del);
}

static void test_query_symbol_not_found(void **state) {
    (void)state;
    elftool_t bin;
    setup_elftool(&bin);
    ft_lstdel(&bin.syms, default_del); // remove the symbol
    bin.syms = NULL;

    elftool_sym_query_t query = {0};
    int r = elftool_sym_query_by_name(&bin, &query, "non_existing_symbol");
    assert_int_not_equal(r, 0);

    free(bin.mem);
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.syms, default_del);
}

static void test_query_write_invalid_size(void **state) {
    (void)state;
    elftool_t bin;
    setup_elftool(&bin);

    sym64_t *sym_wrapper = (sym64_t*)bin.syms->content;
    Elf64_Sym *sym = (Elf64_Sym*)(bin.mem + 0x200);
    sym->st_value = MOCK_VADDR + 0x10;
    sym->st_size = MOCK_SYMBOL_SIZE;
    sym_wrapper->sym_offset = 0x200;

    elftool_sym_query_t query = {0};
    query.op = ELFTOOL_QUERY_WRITE;
    char *new_content = "new_content_with_a_much_longer_size";
    query.sym_content = (uint8_t*)new_content;
    query.sym_size = sizeof("new_content_with_a_much_longer_size");

    int r = elftool_sym_query_by_name(&bin, &query, MOCK_SYMBOL_NAME);
    assert_int_not_equal(r, 0);

    free(bin.mem);
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.syms, default_del);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_query_read),
      cmocka_unit_test(test_query_write),
      cmocka_unit_test(test_query_symbol_not_found),
      cmocka_unit_test(test_query_write_invalid_size),
  };
  cmocka_set_message_output(CM_OUTPUT_XML);
  return cmocka_run_group_tests(tests, NULL, NULL);
}
