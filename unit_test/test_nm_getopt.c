#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <elf.h>
#include "nm_getopt.h"

static void test_nm_getopt(void **state)
{
	(void)state;
	t_nm_opt opt = {0};
	int ac = 3;
	char *av[] = {
		"./nm",
		"-P",
		"-s",
		NULL
	};
	int r = nm_getopt(ac, av, &opt);
	assert_true(r == 0);
	assert_true(opt.config == false);
	assert_true(opt.ehdr == false);
	assert_true(opt.phdr == true);
	assert_true(opt.shdr == false);
	assert_true(opt.syms == true);
	assert_true(opt.hexdump == false);
	assert_true(opt.nm == false);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_nm_getopt),
    };
	cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
