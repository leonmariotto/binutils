#include <stdio.h>

#include "cmocka.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "elftool_getopt.h"
#include <elf.h>

static void test_elftool_getopt(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 3;
  char *av[] = {"./nm", "-P", "-s", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_true(r == 0);
  assert_true(opt.config == false);
  assert_true(opt.ehdr == false);
  assert_true(opt.phdr == true);
  assert_true(opt.shdr == false);
  assert_true(opt.syms == true);
  assert_true(opt.hexdump == false);
  assert_true(opt.nm == false);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_elftool_getopt),
  };
  cmocka_set_message_output(CM_OUTPUT_XML);
  return cmocka_run_group_tests(tests, NULL, NULL);
}
