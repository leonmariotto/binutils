#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "elftool.h"
#include "elftool_transform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_transform_segment_injection_64(void **state) {
    (void) state; /* unused */

    elftool_t bin;

    // 1. Setup mock elftool_t structure
    memset(&bin, 0, sizeof(elftool_t));
    bin.elfclass = ELFCLASS64;
    bin.mem = (uint8_t*)malloc(1024);
    assert_non_null(bin.mem);
    memset(bin.mem, 0, 1024);
    bin.length = 1024;

    // ELF Header
    Elf64_Ehdr *ehdr = get_ehdr64(&bin);
    ehdr->e_ident[EI_CLASS] = ELFCLASS64;
    ehdr->e_phoff = sizeof(Elf64_Ehdr);
    ehdr->e_phnum = 2;
    ehdr->e_phentsize = sizeof(Elf64_Phdr);

    // Program Headers
    Elf64_Phdr *phdr = (Elf64_Phdr *)(bin.mem + ehdr->e_phoff);
    phdr->p_type = PT_LOAD;
    phdr->p_vaddr = 0x400000;

    // phdr list
    phdr64_t phdr_wrapper;
    memset(&phdr_wrapper, 0, sizeof(phdr64_t));
    phdr_wrapper.p_offset = ehdr->e_phoff;
    phdr_wrapper.mem_offset = ehdr->e_phoff;
    phdr_wrapper.bin = &bin;
    ft_lstadd(&bin.phdr, ft_lstnew(&phdr_wrapper, sizeof(phdr_wrapper)));

    // Second phdr of type note
    Elf64_Phdr *phdr2 = (Elf64_Phdr *)(bin.mem + ehdr->e_phoff + sizeof(Elf64_Phdr));
    phdr2->p_type = PT_NOTE;
    phdr64_t phdr_wrapper2;
    memset(&phdr_wrapper2, 0, sizeof(phdr64_t));
    phdr_wrapper2.p_offset = ehdr->e_phoff + sizeof(Elf64_Phdr);
    phdr_wrapper2.mem_offset = ehdr->e_phoff + sizeof(Elf64_Phdr);
    phdr_wrapper2.bin = &bin;
    ft_lstadd(&bin.phdr, ft_lstnew(&phdr_wrapper2, sizeof(phdr_wrapper2)));


    // 2. Setup transform
    elftool_transform_t transform;
    char *injection_code = "beef";
    transform.code = (uint8_t*)injection_code;
    transform.code_len = 4;

    // 3. Call transform function
    int r = elftool_transform_segment_injection(&bin, &transform);
    assert_int_equal(r, 0);

    // Re-initialize pointers after realloc
    ehdr = get_ehdr64(&bin);
    phdr2 = (Elf64_Phdr *)(bin.mem + ehdr->e_phoff + sizeof(Elf64_Phdr));

    // 4. Assert changes
    assert_true(transform.virtual_addr != 0);
    assert_true(phdr2->p_type == PT_LOAD);
    assert_true(phdr2->p_flags == (PF_R | PF_W));


    // Cleanup
    free(bin.mem);
    ft_lstdel(&bin.phdr, default_del);
}

static void test_transform_null_args(void **state) {
    (void) state;
    int r = elftool_transform_segment_injection(NULL, NULL);
    assert_int_not_equal(r, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_transform_segment_injection_64),
        cmocka_unit_test(test_transform_null_args),
    };
    cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
