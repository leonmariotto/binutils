#ifndef ELFTOOL_DUMP_H
#define ELFTOOL_DUMP_H

#include "elftool_getopt.h"
#include "elftool_structs.h"

void elftool_dump_ehdr(elftool_t *bin);
void elftool_dump_phdr(elftool_t *bin);
void elftool_dump_shdr(elftool_t *bin);
void elftool_dump_syms(elftool_t *bin);
void elftool_dump_nm(elftool_t *bin);
void elftool_dump(elftool_opt_t *opt, elftool_t *bin);

#endif
