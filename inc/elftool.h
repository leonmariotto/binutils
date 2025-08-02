#ifndef ELFTOOL_H
#define ELFTOOL_H

#include "liblst.h"
#include "elftool_getopt.h"

// stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// open
#include <fcntl.h>

// mmap
#include <sys/mman.h>

// printf
#include "stdio.h"

// Elf
#include <elf.h>

#include <errno.h>

// __BYTE_ORDER == __LITTLE_ENDIAN and bswap/hton
#include <endian.h>
#include <byteswap.h>
#include <arpa/inet.h>

#define _error(r, str) do {\
	fprintf(stderr, "[ERROR] %s\n", str);\
    r = EFAULT; \
} while (0)

#define _swap16(x) (bin->endian == ELFDATA2MSB ? htons(x) : x)
#define _swap32(x) (bin->endian == ELFDATA2MSB ? htonl(x) : x)

/* Workaround for endianness on 64bit */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define _swap64(x) (bin->endian == ELFDATA2MSB ? bswap_64(x) : x)
#else
#define _swap64(x) (bin->endian == ELFDATA2MSB ? x : bswap_64(x))
#endif

#define get_ehdr(bin) (bin->elfclass == ELFDATA2MSB \
		? ((Elf32_Ehdr*)bin->mem) \
		: ((Elf64_Ehdr*)bin->mem))

#define get_shdr(bin, offset) (bin->elfclass == ELFDATA2MSB \
		? ((Elf32_Shdr*)&bin->mem[offset]) \
		: ((Elf64_Shdr*)&bin->mem[offset]))

#define get_phdr(bin, offset) (bin->elfclass == ELFDATA2MSB \
		? ((Elf32_Phdr*)&bin->mem[offset]) \
		: ((Elf64_Phdr*)&bin->mem[offset]))

typedef struct elftool_s elftool_t;
typedef struct			syms32_s {
	uint16_t		idx;
	Elf32_Sym		*syms;
	Elf32_Shdr		*shdr;
	uint8_t			*mem;
	elftool_t		*bin;
}				syms32_t;

typedef struct			syms64_s {
	uint16_t		idx;
	Elf64_Sym		*syms;
	Elf64_Shdr		*shdr;
	uint8_t			*mem;
	elftool_t	*bin;
}				syms64_t;

typedef struct			phdr32_s {
	uint16_t		idx;
	Elf32_Phdr		*phdr;
	// TODO add 	*mem ptr ?
	elftool_t	*bin;
}				phdr32_t;

typedef struct			phdr64_s {
	uint16_t		idx;
	Elf64_Phdr		*phdr;
	// TODO add 	*mem ptr ?
	elftool_t	*bin;
}				phdr64_t;

typedef struct			shdr32_s {
	uint16_t		idx;
	Elf32_Shdr		*shdr;
	uint8_t			*mem;
	elftool_t	*bin;
}				shdr32_t;

typedef struct			shdr64_s {
	uint16_t		idx;
	Elf64_Shdr		*shdr;
	uint8_t			*mem;
	elftool_t	*bin;
}				shdr64_t;

typedef struct			elftool_s {
	char				*path;
	size_t				length;
	uint8_t 			*mem;
	uint8_t				elfclass;
	uint8_t				endian;
	Elf64_Ehdr			*ehdr64;
	Elf32_Ehdr			*ehdr32;
	t_list				*phdr;
	t_list				*shdr;
	t_list				*syms;
	shdr32_t			*shstrtab32;
	shdr64_t			*shstrtab64;
	shdr32_t			*strtab32;
	shdr64_t			*strtab64;
}				elftool_t;

int		elftool_parse_syms(elftool_t *bin);
int		elftool_parse_shdr(elftool_t *bin);
int		elftool_parse_phdr(elftool_t *bin);
int		elftool_parse_ehdr(elftool_t *bin);
int		elftool_parse(elftool_t *bin);
void	elftool_dump_ehdr(elftool_t *bin);
void	elftool_dump_phdr(elftool_t *bin);
void	elftool_dump_shdr(elftool_t *bin);
void	elftool_dump_syms(elftool_t *bin);
void	elftool_dump_nm(elftool_t *bin);
void	elftool_dump(elftool_opt_t *opt, elftool_t *bin);

int	elftool_getopt(int ac, char **av, elftool_opt_t *opt);
int	elftool(elftool_opt_t *opt);

#endif

