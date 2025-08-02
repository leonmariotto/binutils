#ifndef ELFTOOL_STRUCTS_H
#define ELFTOOL_STRUCTS_H

// Elf
#include <elf.h>

// t_list
#include "liblst.h"

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

#endif

