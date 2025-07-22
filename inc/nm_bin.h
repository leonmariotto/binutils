
#ifndef NM_BINS_H
# define NM_BINS_H

#include "liblst.h"
#include "nm_getopt.h"

// stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// open
#include <fcntl.h>

// mmap
#include <sys/mman.h>

// TODO 
#include "nm_utils.h"

// printf
#include "stdio.h"

// Elf
#include <elf.h>

// __BYTE_ORDER == __LITTLE_ENDIAN and bswap/hton
#include <endian.h>
#include <byteswap.h>
#include <arpa/inet.h>

#define _error(str) do {\
	fprintf(stderr, "[ERROR] mem[%ld]=%x (%s)\n", (uint64_t)offset, bin->mem[offset], str);\
	r = -1; \
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

typedef struct s_nm_bin t_nm_bin;
typedef struct			s_nm_syms32 {
	uint16_t	idx;
	Elf32_Sym	*syms;
	Elf32_Shdr	*shdr;
	uint8_t		*mem;
	t_nm_bin	*bin;
}				t_nm_syms32;

typedef struct			s_nm_syms64 {
	uint16_t	idx;
	Elf64_Sym	*syms;
	Elf64_Shdr	*shdr;
	uint8_t		*mem;
	t_nm_bin	*bin;
}				t_nm_syms64;

typedef struct			s_nm_phdr32 {
	uint16_t	idx;
	Elf32_Phdr	*phdr;
	// TODO add *mem ptr ?
	t_nm_bin	*bin;
}				t_nm_phdr32;

typedef struct			s_nm_phdr64 {
	uint16_t	idx;
	Elf64_Phdr	*phdr;
	// TODO add *mem ptr ?
	t_nm_bin	*bin;
}				t_nm_phdr64;

typedef struct			s_nm_shdr32 {
	uint16_t	idx;
	Elf32_Shdr	*shdr;
	uint8_t		*mem;
	t_nm_bin	*bin;
}				t_nm_shdr32;

typedef struct			s_nm_shdr64 {
	uint16_t	idx;
	Elf64_Shdr	*shdr;
	uint8_t		*mem;
	t_nm_bin	*bin;
}				t_nm_shdr64;

typedef struct			s_nm_bin {
	char		*path;
	size_t		length;
	uint8_t 	*mem;
	uint8_t		elfclass;
	uint8_t		endian;
	Elf64_Ehdr	*ehdr64;
	Elf32_Ehdr	*ehdr32;
	t_list		*phdr;
	t_list		*shdr;
	t_list		*syms;
	t_nm_shdr32	*shstrtab32;
	t_nm_shdr64	*shstrtab64;
	t_nm_shdr32	*strtab32;
	t_nm_shdr64	*strtab64;
}				t_nm_bin;

int	nm_bin_parse_syms(t_nm_bin *bin);
int	nm_bin_parse_shdr(t_nm_bin *bin);
int	nm_bin_parse_phdr(t_nm_bin *bin);
int	nm_bin_parse_ehdr(t_nm_bin *bin);

int	nm_bin_parse(t_nm_bin *bin);

void	nm_bin_dump_ehdr(t_nm_bin *bin);
void	nm_bin_dump_phdr(t_nm_bin *bin);
void	nm_bin_dump_shdr(t_nm_bin *bin);
void	nm_bin_dump_syms(t_nm_bin *bin);
void	nm_bin_dump_nm(t_nm_bin *bin);

void	nm_bin_dump(t_nm_opt *opt, t_nm_bin *bin);

int	nm_bin(t_nm_opt *opt, t_nm_bin *bin);

#endif


