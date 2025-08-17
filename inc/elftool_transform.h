#ifndef ELFTOOL_TRANSFORM_H
#define ELFTOOL_TRANSFORM_H

#include "elftool_structs.h"

typedef struct elftool_transform_s {
    uint8_t *code;             /**< Pointer to code that will be injected */
    uint64_t code_len;         /**< Length of code to inject. */
    uint64_t code_file_offset; /**< Out, file offset where the code has been
                                  written. */
    uint64_t
        phdr_file_offset;  /**< Out, file offset where the phdr entry reside. */
    uint64_t virtual_addr; /**< Out, virtual address where the code will be
                            * loaded. Need to add base_addr to compute the
                            * runtime address. */
} elftool_transform_t;

/*
 * @brief Data Segment injection.
 * Inject a transform->code into a new PF_R PF_W segment that will be loaded at
 * runtime. Caller can rely on transform->virtual_addr value that is filled by
 * this function to retrieve the runtime address of injected data. (adding
 * base_addr). This function override the first PT_NOTE segment found.
 *
 * @param           bin: binary to modify
 * @param           transform: opt struct describing changes.
 * @return          0 for success
 * */
int elftool_transform_segment_injection(elftool_t *bin,
                                        elftool_transform_t *transform);

#endif /* ELFTOOL_TRANSFORM_H */
