#ifndef ELFTOOL_PARSE_H
#define ELFTOOL_PARSE_H

#include "elftool_structs.h"

int elftool_parse_syms(elftool_t *bin);
int elftool_parse_shdr(elftool_t *bin);
int elftool_parse_phdr(elftool_t *bin);
int elftool_parse_ehdr(elftool_t *bin);
int elftool_parse(elftool_t *bin);

#endif
