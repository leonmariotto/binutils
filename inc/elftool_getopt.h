#ifndef ELFTOOL_GETOPT_H
#define ELFTOOL_GETOPT_H

#include "liblst.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NM_ARCH_M64 1
#define NM_ARCH_M32 2

typedef struct elftool_opt_s {
  bool config;
  bool ehdr;
  bool phdr;
  bool shdr;
  bool syms;
  bool hexdump;
  bool nm;
  bool out;
  bool test;
  char *outfile;
  t_list *bins;
} elftool_opt_t;

void elftool_printopt(elftool_opt_t *opt);
int elftool_getopt(int ac, char **av, elftool_opt_t *opt);

#endif
