#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <elf.h>
#include "elftool.h"

static void test_elftool_parse64_ehdr(void **state)
{
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
		.mem = (void*)&hdr,
		.length = 200,
	};
	int r = elftool_parse_ehdr(&bin);
	assert_true(r == 0);
	assert_true(bin.elfclass == ELFCLASS64);
	assert_true(bin.endian == ELFDATA2LSB);
	assert_ptr_equal(bin.ehdr64, &hdr);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_elftool_parse64_ehdr),
    };
	cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
