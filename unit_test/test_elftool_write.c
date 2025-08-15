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



int main(void) {
    const struct CMUnitTest tests[] = {
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
