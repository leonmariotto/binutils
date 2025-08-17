#ifndef ELFTOOL_WRITE_H
#define ELFTOOL_WRITE_H

#include "elftool_structs.h"

/**
 * @brief Writes the ELF file to disk.
 *
 * This function writes the ELF file to disk.
 *
 * @param bin The elftool structure.
 * @param file_out The path to the output file.
 * @return 0 on success, non-zero on failure.
 */
int elftool_write(elftool_t *bin, char *file_out);

#endif
