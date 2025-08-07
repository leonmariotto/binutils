#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include "cmocka.h"

#include <elf.h>
#include "elftool_getopt.h"

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

static void test_elftool_getopt_help(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--help", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 1);
}

static void test_elftool_getopt_h(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "-h", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 1);
}

static void test_elftool_getopt_config(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--config", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  assert_true(opt.config == true);
}

static void test_elftool_getopt_ehdr(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--elf-header", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  assert_true(opt.ehdr == true);
}

static void test_elftool_getopt_section(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--section-header", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  assert_true(opt.shdr == true);
}

static void test_elftool_getopt_hexdump(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--hexdump", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  assert_true(opt.hexdump == true);
}

static void test_elftool_getopt_nm(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--nm", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  assert_true(opt.nm == true);
}

static void test_elftool_getopt_test(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 2;
  char *av[] = {"./nm", "--test", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  // assert_true(opt.test == true); // test is commented out in src
}

static void test_elftool_getopt_multiple_bins(void **state) {
  (void)state;
  elftool_opt_t opt = {0};
  int ac = 4;
  char *av[] = {"./nm", "bin1", "bin2", "bin3", NULL};
  int r = elftool_getopt(ac, av, &opt);
  assert_int_equal(r, 0);
  assert_non_null(opt.bins);
  elftool_printopt(&opt);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_elftool_getopt),
      cmocka_unit_test(test_elftool_getopt_help),
      cmocka_unit_test(test_elftool_getopt_h),
      cmocka_unit_test(test_elftool_getopt_config),
      cmocka_unit_test(test_elftool_getopt_ehdr),
      cmocka_unit_test(test_elftool_getopt_section),
      cmocka_unit_test(test_elftool_getopt_hexdump),
      cmocka_unit_test(test_elftool_getopt_nm),
      cmocka_unit_test(test_elftool_getopt_test),
      cmocka_unit_test(test_elftool_getopt_multiple_bins),
  };
  cmocka_set_message_output(CM_OUTPUT_XML);
  return cmocka_run_group_tests(tests, NULL, NULL);
}
