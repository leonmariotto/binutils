#ifndef ELFTOOL_STRUCTS_H
#define ELFTOOL_STRUCTS_H

// Elf
#include <elf.h>

// list_t
#include "liblst.h"

typedef struct elftool_s elftool_t;
typedef struct sym32_s {
    uint16_t idx;
    Elf32_Off sh_offset;
    Elf32_Off mem_offset;
    Elf32_Off sym_offset;
    elftool_t *bin;
} sym32_t;

#define get_sym32_ent(sym32) \
    ((Elf32_Sym*)&(((sym32_t*)sym32)->bin)->mem[((sym32_t*)sym32)->sym_offset])
#define get_sym32_shdr(sym32) \
    ((Elf32_Shdr*)&(((sym32_t*)sym32)->bin)->mem[((sym32_t*)sym32)->sh_offset])
#define get_sym32_mem(sym32) \
    ((uint8_t*)&(((sym32_t*)sym32)->bin)->mem[((sym32_t*)sym32)->mem_offset])

typedef struct sym64_s {
    uint16_t idx;
    Elf64_Off sh_offset;
    Elf64_Off mem_offset;
    Elf64_Off sym_offset;
    elftool_t *bin;
} sym64_t;

#define get_sym64_ent(sym64) \
    ((Elf64_Sym*)&(((sym64_t*)sym64)->bin)->mem[((sym64_t*)sym64)->sym_offset])
#define get_sym64_shdr(sym64) \
    ((Elf64_Shdr*)&(((sym64_t*)sym64)->bin)->mem[((sym64_t*)sym64)->sh_offset])
#define get_sym64_mem(sym64) \
    ((uint8_t*)&(((sym64_t*)sym64)->bin)->mem[((sym64_t*)sym64)->mem_offset])

typedef struct phdr32_s {
    uint16_t idx;
    Elf32_Off p_offset;
    Elf32_Off mem_offset;
    elftool_t *bin;
} phdr32_t;

#define get_phdr32_ent(phdr32) \
    ((Elf32_Phdr*)&(((phdr32_t*)phdr32)->bin)->mem[((phdr32_t*)phdr32)->p_offset])
#define get_phdr32_mem(phdr32) \
    ((uint8_t*)&(((phdr32_t*)phdr32)->bin)->mem[((phdr32_t*)phdr32)->mem_offset])

typedef struct phdr64_s {
    uint16_t idx;
    Elf64_Off p_offset;
    Elf64_Off mem_offset;
    elftool_t *bin;
} phdr64_t;

#define get_phdr64_ent(phdr64) \
    ((Elf64_Phdr*)&(((phdr64_t*)phdr64)->bin)->mem[((phdr64_t*)phdr64)->p_offset])
#define get_phdr64_mem(phdr64) \
    ((uint8_t*)&(((phdr64_t*)phdr64)->bin)->mem[((phdr64_t*)phdr64)->mem_offset])

typedef struct shdr32_s {
    uint16_t idx;
    Elf32_Off sh_offset;
    Elf32_Off mem_offset;
    elftool_t *bin;
} shdr32_t;

#define get_shdr32_ent(shdr32) \
    ((Elf32_Shdr*)&(((shdr32_t*)shdr32)->bin)->mem[((shdr32_t*)shdr32)->sh_offset])
#define get_shdr32_mem(shdr32) \
    ((uint8_t*)&(((shdr32_t*)shdr32)->bin)->mem[((shdr32_t*)shdr32)->mem_offset])

typedef struct shdr64_s {
    uint16_t idx;
    Elf64_Off sh_offset;
    Elf64_Off mem_offset;
    elftool_t *bin;
} shdr64_t;

#define get_shdr64_ent(shdr64) \
    ((Elf64_Shdr*)&(((shdr64_t*)shdr64)->bin)->mem[((shdr64_t*)shdr64)->sh_offset])
#define get_shdr64_mem(shdr64) \
    ((uint8_t*)&(((shdr64_t*)shdr64)->bin)->mem[((shdr64_t*)shdr64)->mem_offset])

typedef struct elftool_s {
    char *path;
    size_t length;
    uint8_t *mem;
    uint8_t elfclass;
    uint8_t endian;
    list_t *phdr;
    list_t *shdr;
    list_t *syms;
    shdr32_t *shstrtab32;
    shdr64_t *shstrtab64;
    shdr32_t *strtab32;
    shdr64_t *strtab64;
} elftool_t;

#define get_ehdr64(bin) \
    ((Elf64_Ehdr*)(bin)->mem)
#define get_ehdr32(bin) \
    ((Elf32_Ehdr*)(bin)->mem)

#endif
