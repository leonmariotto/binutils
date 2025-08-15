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

static void test_transform_section_injection_64(void **state) {
    (void) state; /* unused */

    elftool_t bin;
    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr1, phdr2;
    Elf64_Shdr shdr1, shdr2;

    // 1. Setup mock elftool_t structure
    memset(&bin, 0, sizeof(elftool_t));
    bin.elfclass = ELFCLASS64;
    bin.ehdr64 = &ehdr;

    // ELF Header
    memset(&ehdr, 0, sizeof(Elf64_Ehdr));
    ehdr.e_shnum = 2;
    ehdr.e_shentsize = sizeof(Elf64_Shdr);
		ehdr.e_shstrndx = 1;

    // Program Headers
    phdr64_t phdr_wrapper1, phdr_wrapper2;
    memset(&phdr_wrapper1, 0, sizeof(phdr64_t));
    phdr_wrapper1.phdr = &phdr1;
    memset(&phdr1, 0, sizeof(Elf64_Phdr));
    phdr1.p_type = PT_LOAD;
    phdr1.p_offset = 0;
    phdr1.p_vaddr = 0x400000;
    phdr1.p_filesz = 0x1000;
    phdr1.p_memsz = 0x1000;
    ft_lstadd(&bin.phdr, ft_lstnew(&phdr_wrapper1, sizeof(phdr_wrapper1)));

    memset(&phdr_wrapper2, 0, sizeof(phdr64_t));
    phdr_wrapper2.phdr = &phdr2;
    memset(&phdr2, 0, sizeof(Elf64_Phdr));
    phdr2.p_type = PT_LOAD;
    phdr2.p_offset = 0x1000;
    phdr2.p_vaddr = 0x401000;
    phdr2.p_filesz = 0x1000;
    phdr2.p_memsz = 0x1000;
    ft_lstadd(&bin.phdr, ft_lstnew(&phdr_wrapper2, sizeof(phdr_wrapper2)));


    // Section Headers
    shdr64_t shdr_wrapper1, shdr_wrapper2;
    memset(&shdr_wrapper1, 0, sizeof(shdr64_t));
    shdr_wrapper1.shdr = &shdr1;
		shdr_wrapper1.idx = 0;
    memset(&shdr1, 0, sizeof(Elf64_Shdr));
    shdr1.sh_offset = 0x100;
    shdr1.sh_size = 0x100;
    ft_lstadd(&bin.shdr, ft_lstnew(&shdr_wrapper1, sizeof(shdr_wrapper1)));

    memset(&shdr_wrapper2, 0, sizeof(shdr64_t));
    shdr_wrapper2.shdr = &shdr2;
		shdr_wrapper2.idx = 1;
    memset(&shdr2, 0, sizeof(Elf64_Shdr));
    shdr2.sh_offset = 0x1100; // Within the second PT_LOAD segment
    shdr2.sh_size = 0x100;
    ft_lstadd(&bin.shdr, ft_lstnew(&shdr_wrapper2, sizeof(shdr_wrapper2)));

    // 2. Setup transform
    elftool_transform_t transform;
    char *injection_code = "beef";
    transform.code = (uint8_t*)injection_code;
    transform.code_len = 4;
    transform.type = NM_SECTION_INJECT;

    // 3. Call transform function
    int r = nm_bin_transform_section_injection(&bin, &transform);
    assert_int_equal(r, 0);

    // 4. Assert changes
    assert_int_equal(bin.ehdr64->e_shnum, 3);

    // find the injected section
    list_t* head = bin.shdr;
    int found = 0;
    while(head) {
        shdr64_t* current_shdr = (shdr64_t*)head->content;
        if (current_shdr->idx == 2) {
            assert_int_equal(current_shdr->shdr->sh_size, 4);
            found = 1;
        }
        head = head->next;
    }
    assert_int_equal(found, 1);


    // Cleanup
    // Note: the transform function allocates memory for the new section,
    // but the list deletion will not free it since it's a shallow copy.
    // This is a memory leak in the current design of the code being tested.
    // We will ignore it for the purpose of this test.
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.shdr, default_del);
}

static void test_transform_null_args(void **state) {
    (void) state;
    int r = nm_bin_transform_section_injection(NULL, NULL);
    assert_int_equal(r, -1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_transform_section_injection_64),
        cmocka_unit_test(test_transform_null_args),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
