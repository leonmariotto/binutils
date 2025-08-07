#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "elftool.h"
#include "elftool_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static void test_elftool_write_simple_64(void **state) {
    (void) state; /* unused */

    elftool_t bin;
    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr;
    Elf64_Shdr shdr;
    char *section_content = "Hello, World!";
    int section_len = strlen(section_content) + 1; // Include null terminator

    // 1. Setup mock elftool_t structure
    memset(&bin, 0, sizeof(elftool_t));
    bin.elfclass = ELFCLASS64;
    bin.ehdr64 = &ehdr;
    bin.phdr = NULL;
    bin.shdr = NULL;

    // ELF Header
    memset(&ehdr, 0, sizeof(Elf64_Ehdr));
    ehdr.e_ident[EI_MAG0] = ELFMAG0;
    ehdr.e_ident[EI_MAG1] = ELFMAG1;
    ehdr.e_ident[EI_MAG2] = ELFMAG2;
    ehdr.e_ident[EI_MAG3] = ELFMAG3;
    ehdr.e_ident[EI_CLASS] = ELFCLASS64;
    ehdr.e_type = ET_EXEC;
    ehdr.e_machine = EM_X86_64;
    ehdr.e_version = EV_CURRENT;
    ehdr.e_entry = 0x400080;
    ehdr.e_phoff = sizeof(Elf64_Ehdr);
    ehdr.e_shoff = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);
    ehdr.e_ehsize = sizeof(Elf64_Ehdr);
    ehdr.e_phentsize = sizeof(Elf64_Phdr);
    ehdr.e_phnum = 1;
    ehdr.e_shentsize = sizeof(Elf64_Shdr);
    ehdr.e_shnum = 1;
    ehdr.e_shstrndx = 0;

    // Section Header
    shdr64_t shdr_wrapper;
    memset(&shdr_wrapper, 0, sizeof(shdr64_t));
    shdr_wrapper.shdr = &shdr;
    shdr_wrapper.mem = (uint8_t *)section_content;
    memset(&shdr, 0, sizeof(Elf64_Shdr));
    shdr.sh_name = 0;
    shdr.sh_type = SHT_PROGBITS;
    shdr.sh_flags = SHF_ALLOC;
    shdr.sh_addr = 0x400080;
    shdr.sh_offset = ehdr.e_shoff + sizeof(Elf64_Shdr); // Content after header
    shdr.sh_size = section_len;
    ft_lstadd(&bin.shdr, ft_lstnew(&shdr_wrapper, sizeof(shdr_wrapper)));

    // Program Header
    phdr64_t phdr_wrapper;
    memset(&phdr_wrapper, 0, sizeof(phdr64_t));
    phdr_wrapper.phdr = &phdr;
    memset(&phdr, 0, sizeof(Elf64_Phdr));
    phdr.p_type = PT_LOAD;
    phdr.p_offset = 0;
    phdr.p_vaddr = 0x400000;
    phdr.p_paddr = 0x400000;
    phdr.p_filesz = shdr.sh_offset + shdr.sh_size; // Cover all headers and content
    phdr.p_memsz = phdr.p_filesz;
    phdr.p_flags = PF_R | PF_X;
    ft_lstadd(&bin.phdr, ft_lstnew(&phdr_wrapper, sizeof(phdr_wrapper)));


    // 2. Call elftool_write
    char *tmp_file = "test_elf_out.bin";
    int r = elftool_write(&bin, tmp_file);
    assert_int_equal(r, 0);

    // 3. Verify file content (basic check)
    int fd = open(tmp_file, O_RDONLY);
    assert_true(fd > 0);
    char buf[section_len];
    lseek(fd, shdr.sh_offset, SEEK_SET);
    read(fd, buf, section_len);
    assert_memory_equal(buf, section_content, section_len);
    close(fd);

    // 4. Cleanup
    unlink(tmp_file);
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.shdr, default_del);
}

static void test_elftool_write_simple_32(void **state) {
    (void) state; /* unused */

    elftool_t bin;
    Elf32_Ehdr ehdr;
    Elf32_Phdr phdr;
    Elf32_Shdr shdr;
    char *section_content = "Hello, World!";
    int section_len = strlen(section_content) + 1; // Include null terminator

    // 1. Setup mock elftool_t structure
    memset(&bin, 0, sizeof(elftool_t));
    bin.elfclass = ELFCLASS32;
    bin.ehdr32 = &ehdr;
    bin.phdr = NULL;
    bin.shdr = NULL;

    // ELF Header
    memset(&ehdr, 0, sizeof(Elf32_Ehdr));
    ehdr.e_ident[EI_MAG0] = ELFMAG0;
    ehdr.e_ident[EI_MAG1] = ELFMAG1;
    ehdr.e_ident[EI_MAG2] = ELFMAG2;
    ehdr.e_ident[EI_MAG3] = ELFMAG3;
    ehdr.e_ident[EI_CLASS] = ELFCLASS32;
    ehdr.e_type = ET_EXEC;
    ehdr.e_machine = EM_386;
    ehdr.e_version = EV_CURRENT;
    ehdr.e_entry = 0x8048080;
    ehdr.e_phoff = sizeof(Elf32_Ehdr);
    ehdr.e_shoff = sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr);
    ehdr.e_ehsize = sizeof(Elf32_Ehdr);
    ehdr.e_phentsize = sizeof(Elf32_Phdr);
    ehdr.e_phnum = 1;
    ehdr.e_shentsize = sizeof(Elf32_Shdr);
    ehdr.e_shnum = 1;
    ehdr.e_shstrndx = 0;

    // Section Header
    shdr32_t shdr_wrapper;
    memset(&shdr_wrapper, 0, sizeof(shdr32_t));
    shdr_wrapper.shdr = &shdr;
    shdr_wrapper.mem = (uint8_t *)section_content;
    memset(&shdr, 0, sizeof(Elf32_Shdr));
    shdr.sh_name = 0;
    shdr.sh_type = SHT_PROGBITS;
    shdr.sh_flags = SHF_ALLOC;
    shdr.sh_addr = 0x8048080;
    shdr.sh_offset = ehdr.e_shoff + sizeof(Elf32_Shdr); // Content after header
    shdr.sh_size = section_len;
    ft_lstadd(&bin.shdr, ft_lstnew(&shdr_wrapper, sizeof(shdr_wrapper)));

    // Program Header
    phdr32_t phdr_wrapper;
    memset(&phdr_wrapper, 0, sizeof(phdr32_t));
    phdr_wrapper.phdr = &phdr;
    memset(&phdr, 0, sizeof(Elf32_Phdr));
    phdr.p_type = PT_LOAD;
    phdr.p_offset = 0;
    phdr.p_vaddr = 0x8048000;
    phdr.p_paddr = 0x8048000;
    phdr.p_filesz = shdr.sh_offset + shdr.sh_size; // Cover all headers and content
    phdr.p_memsz = phdr.p_filesz;
    phdr.p_flags = PF_R | PF_X;
    ft_lstadd(&bin.phdr, ft_lstnew(&phdr_wrapper, sizeof(phdr_wrapper)));


    // 2. Call elftool_write
    char *tmp_file = "test_elf_out32.bin";
    int r = elftool_write(&bin, tmp_file);
    assert_int_equal(r, 0);

    // 3. Verify file content (basic check)
    int fd = open(tmp_file, O_RDONLY);
    assert_true(fd > 0);
    char buf[section_len];
    lseek(fd, shdr.sh_offset, SEEK_SET);
    read(fd, buf, section_len);
    assert_memory_equal(buf, section_content, section_len);
    close(fd);

    // 4. Cleanup
    unlink(tmp_file);
    ft_lstdel(&bin.phdr, default_del);
    ft_lstdel(&bin.shdr, default_del);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_elftool_write_simple_64),
        cmocka_unit_test(test_elftool_write_simple_32),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
