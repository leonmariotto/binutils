#ifndef ELFTOOL_GETOPT_H
#define ELFTOOL_GETOPT_H

#include "liblst.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief 64-bit architecture for nm.
 */
#define NM_ARCH_M64 1
/**
 * @brief 32-bit architecture for nm.
 */
#define NM_ARCH_M32 2

/**
 * @brief Options for the elftool.
 *
 * This struct holds the options for the elftool.
 */
typedef struct elftool_opt_s {
	bool config;    /**< Configuration flag. */
	bool ehdr;      /**< ELF header flag. */
	bool phdr;      /**< Program header flag. */
	bool shdr;      /**< Section header flag. */
	bool syms;      /**< Symbols flag. */
	bool hexdump;   /**< Hexdump flag. */
	bool nm;        /**< nm style flag. */
	bool out;       /**< Output flag. */
	bool test;      /**< Test flag. */
	char *outfile;  /**< Output file name. */
	list_t *bins;   /**< List of binaries. */
} elftool_opt_t;

/**
 * @brief Prints the options.
 *
 * @param opt The options for the elftool.
 */
void elftool_printopt(elftool_opt_t *opt);
/**
 * @brief Parses the command line options.
 *
 * @param ac The number of arguments.
 * @param av The arguments.
 * @param opt The options for the elftool.
 * @return 0 on success, non-zero on failure.
 */
int elftool_getopt(int ac, char **av, elftool_opt_t *opt);

#endif
