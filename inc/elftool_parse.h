#ifndef ELFTOOL_PARSE_H
#define ELFTOOL_PARSE_H

#include "elftool_structs.h"

/**
 * @brief Parses the symbols.
 *
 * @param bin The elftool structure.
 * @return 0 on success, non-zero on failure.
 */
int elftool_parse_syms(elftool_t *bin);
/**
 * @brief Parses the section headers.
 *
 * @param bin The elftool structure.
 * @return 0 on success, non-zero on failure.
 */
int elftool_parse_shdr(elftool_t *bin);
/**
 * @brief Parses the program headers.
 *
 * @param bin The elftool structure.
 * @return 0 on success, non-zero on failure.
 */
int elftool_parse_phdr(elftool_t *bin);
/**
 * @brief Parses the ELF header.
 *
 * @param bin The elftool structure.
 * @return 0 on success, non-zero on failure.
 */
int elftool_parse_ehdr(elftool_t *bin);
/**
 * @brief Parses the ELF file.
 *
 * @param bin The elftool structure.
 * @return 0 on success, non-zero on failure.
 */
int elftool_parse(elftool_t *bin);

#endif
