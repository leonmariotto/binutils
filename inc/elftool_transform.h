#ifndef ELFTOOL_TRANSFORM_H
#define ELFTOOL_TRANSFORM_H

#include "elftool_structs.h"

/**
 * @brief Represents a transform operation.
 *
 * This struct holds information about a transform operation.
 */
typedef struct elftool_transform_s {
	uint8_t *code;       /**< Pointer to code to be injected. */
	uint64_t code_len;   /**< Length of code to inject. */
	uint64_t code_file_offset; /**< Out, file offset where the code has been written. */
	uint64_t phdr_file_offset; /**< Out, file offset where the phdr entry reside. */
	uint64_t virtual_addr;     /**< Out, virtual address where the code will be loaded. Add base_addr for runtime address. */
} elftool_transform_t;

/**
 * @brief Injects code into a new data segment.
 *
 * Injects code into a new PF_R|PF_W segment.
 * Overrides the first PT_NOTE segment found.
 *
 * @param bin The binary to modify.
 * @param transform The transform struct describing changes.
 * @return 0 on success, non-zero on failure.
 */
int elftool_transform_segment_injection(elftool_t *bin,
					elftool_transform_t *transform);

#endif /* ELFTOOL_TRANSFORM_H */
