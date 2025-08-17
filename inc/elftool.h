#ifndef ELFTOOL_H
#define ELFTOOL_H

#include "elftool_getopt.h"
#include "liblst.h"

// stat
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// open
#include <fcntl.h>

// mmap
#include <sys/mman.h>

// printf
#include "stdio.h"

#include <errno.h>

#include "elftool_structs.h"

// __BYTE_ORDER == __LITTLE_ENDIAN and bswap/hton
#include <arpa/inet.h>
#include <byteswap.h>
#include <endian.h>

/**
 * @brief Prints an error message to stderr.
 *
 * @param r The return value to set.
 * @param str The error message to print.
 */
#define _error(r, str)                                                         \
  do {                                                                         \
    fprintf(stderr, "[ERROR] %s\n", str);                                      \
    r = EFAULT;                                                                \
  } while (0)

/**
 * @brief Swaps a 16-bit value if necessary.
 *
 * @param x The 16-bit value to swap.
 */
#define _swap16(x) (bin->endian == ELFDATA2MSB ? htons(x) : x)
/**
 * @brief Swaps a 32-bit value if necessary.
 *
 * @param x The 32-bit value to swap.
 */
#define _swap32(x) (bin->endian == ELFDATA2MSB ? htonl(x) : x)

/* Workaround for endianness on 64bit */
/**
 * @brief Swaps a 64-bit value if necessary.
 *
 * @param x The 64-bit value to swap.
 */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define _swap64(x) (bin->endian == ELFDATA2MSB ? bswap_64(x) : x)
#else
#define _swap64(x) (bin->endian == ELFDATA2MSB ? x : bswap_64(x))
#endif

/**
 * @brief Main function for the elftool.
 *
 * This function takes options and performs actions.
 *
 * @param opt The options for the elftool.
 * @return 0 on success, non-zero on failure.
 */
int elftool(elftool_opt_t *opt);

#endif
