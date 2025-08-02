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

#define _error(r, str)                                                         \
  do {                                                                         \
    fprintf(stderr, "[ERROR] %s\n", str);                                      \
    r = EFAULT;                                                                \
  } while (0)

#define _swap16(x) (bin->endian == ELFDATA2MSB ? htons(x) : x)
#define _swap32(x) (bin->endian == ELFDATA2MSB ? htonl(x) : x)

/* Workaround for endianness on 64bit */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define _swap64(x) (bin->endian == ELFDATA2MSB ? bswap_64(x) : x)
#else
#define _swap64(x) (bin->endian == ELFDATA2MSB ? x : bswap_64(x))
#endif

int elftool(elftool_opt_t *opt);

#endif
