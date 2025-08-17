#ifndef ELFTOOL_LOAD_H
#define ELFTOOL_LOAD_H

#include "elftool_structs.h"

/**
 * @brief Loads an ELF file into memory.
 *
 * This function loads an ELF file into memory.
 *
 * @param bin The elftool structure.
 * @param bin_path The path to the ELF file.
 * @return 0 on success, non-zero on failure.
 */
int elftool_load(elftool_t *bin, char *bin_path);

#endif
