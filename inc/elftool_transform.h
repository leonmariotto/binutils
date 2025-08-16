#ifndef ELFTOOL_TRANSFORM_H
#define ELFTOOL_TRANSFORM_H

#include "elftool_structs.h"

#define NM_CODECAVE_INJECT 0
#define NM_SILVIO_INJECT 1
#define NM_SECTION_INJECT 2

// TODO add sym
typedef struct elftool_transform_s {
    uint8_t type;
    uint8_t *code;
    uint64_t code_len;
    uint64_t code_file_offset;
    uint64_t phdr_file_offset;
} elftool_transform_t;

int elftool_transform_section_injection(elftool_t *bin,
                                        elftool_transform_t *transform);
int elftool_transform_silvio_injection(elftool_t *bin,
                                       elftool_transform_t *transform);
int elftool_transform_codecave_injection(elftool_t *bin,
                                         elftool_transform_t *transform);

int elftool_transform(elftool_t *bin, elftool_transform_t *transform);

#endif /* ELFTOOL_TRANSFORM_H */
