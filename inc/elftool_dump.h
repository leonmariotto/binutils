#ifndef ELFTOOL_DUMP_H
#define ELFTOOL_DUMP_H

#include "elftool_getopt.h"
#include "elftool_structs.h"

/**
 * @brief Dumps the ELF header.
 *
 * @param bin The elftool structure.
 */
void elftool_dump_ehdr(elftool_t *bin);
/**
 * @brief Dumps the program headers.
 *
 * @param bin The elftool structure.
 */
void elftool_dump_phdr(elftool_t *bin);
/**
 * @brief Dumps the section headers.
 *
 * @param bin The elftool structure.
 */
void elftool_dump_shdr(elftool_t *bin);
/**
 * @brief Dumps the symbols.
 *
 * @param bin The elftool structure.
 */
void elftool_dump_syms(elftool_t *bin);
/**
 * @brief Dumps the symbols in nm style.
 *
 * @param bin The elftool structure.
 */
void elftool_dump_nm(elftool_t *bin);
/**
 * @brief Dumps information based on options.
 *
 * @param opt The options for the elftool.
 * @param bin The elftool structure.
 */
void elftool_dump(elftool_opt_t *opt, elftool_t *bin);

#endif
