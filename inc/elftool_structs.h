#ifndef ELFTOOL_STRUCTS_H
#define ELFTOOL_STRUCTS_H

// Elf
#include <elf.h>

// list_t
#include "liblst.h"

typedef struct elftool_s elftool_t;

/**
 * @brief Represents a 32-bit symbol.
 *
 * This struct holds information about a 32-bit symbol.
 */
typedef struct sym32_s {
	uint16_t idx;       /**< Index of the symbol. */
	Elf32_Off sh_offset;  /**< Section header offset. */
	Elf32_Off mem_offset; /**< Memory offset. */
	Elf32_Off sym_offset; /**< Symbol table offset. */
	char *name;           /**< Name of the symbol. */
	elftool_t *bin;       /**< Pointer to elftool structure. */
} sym32_t;

/**
 * @brief Gets the 32-bit symbol table entry.
 *
 * @param sym32 The 32-bit symbol.
 */
#define get_sym32_ent(sym32)                                                   \
	((Elf32_Sym *)&(((sym32_t *)sym32)->bin)                                   \
			 ->mem[((sym32_t *)sym32)->sym_offset])
/**
 * @brief Gets the 32-bit section header.
 *
 * @param sym32 The 32-bit symbol.
 */
#define get_sym32_shdr(sym32)                                                  \
	((Elf32_Shdr *)&(((sym32_t *)sym32)->bin)                                  \
			  ->mem[((sym32_t *)sym32)->sh_offset])
/**
 * @brief Gets the memory address of the 32-bit symbol.
 *
 * @param sym32 The 32-bit symbol.
 */
#define get_sym32_mem(sym32)                                                   \
	((uint8_t *)&(((sym32_t *)sym32)->bin)                                     \
			 ->mem[((sym32_t *)sym32)->mem_offset])

/**
 * @brief Represents a 64-bit symbol.
 *
 * This struct holds information about a 64-bit symbol.
 */
typedef struct sym64_s {
	uint16_t idx;       /**< Index of the symbol. */
	Elf64_Off sh_offset;  /**< Section header offset. */
	Elf64_Off mem_offset; /**< Memory offset. */
	Elf64_Off sym_offset; /**< Symbol table offset. */
	char *name;           /**< Name of the symbol. */
	elftool_t *bin;       /**< Pointer to elftool structure. */
} sym64_t;

/**
 * @brief Gets the 64-bit symbol table entry.
 *
 * @param sym64 The 64-bit symbol.
 */
#define get_sym64_ent(sym64)                                                   \
	((Elf64_Sym *)&(((sym64_t *)sym64)->bin)                                   \
			 ->mem[((sym64_t *)sym64)->sym_offset])
/**
 * @brief Gets the 64-bit section header.
 *
 * @param sym64 The 64-bit symbol.
 */
#define get_sym64_shdr(sym64)                                                  \
	((Elf64_Shdr *)&(((sym64_t *)sym64)->bin)                                  \
			  ->mem[((sym64_t *)sym64)->sh_offset])
/**
 * @brief Gets the memory address of the 64-bit symbol.
 *
 * @param sym64 The 64-bit symbol.
 */
#define get_sym64_mem(sym64)                                                   \
	((uint8_t *)&(((sym64_t *)sym64)->bin)                                     \
			 ->mem[((sym64_t *)sym64)->mem_offset])

/**
 * @brief Represents a 32-bit program header.
 *
 * This struct holds info about a 32-bit program header.
 */
typedef struct phdr32_s {
	uint16_t idx;       /**< Index of the program header. */
	Elf32_Off p_offset;   /**< Program header offset. */
	Elf32_Off mem_offset; /**< Memory offset. */
	elftool_t *bin;       /**< Pointer to elftool structure. */
} phdr32_t;

/**
 * @brief Gets the 32-bit program header entry.
 *
 * @param phdr32 The 32-bit program header.
 */
#define get_phdr32_ent(phdr32)                                                 \
	((Elf32_Phdr *)&(((phdr32_t *)phdr32)->bin)                                \
			  ->mem[((phdr32_t *)phdr32)->p_offset])
/**
 * @brief Gets the memory address of the 32-bit program header.
 *
 * @param phdr32 The 32-bit program header.
 */
#define get_phdr32_mem(phdr32)                                                 \
	((uint8_t *)&(((phdr32_t *)phdr32)->bin)                                   \
			 ->mem[((phdr32_t *)phdr32)->mem_offset])

/**
 * @brief Represents a 64-bit program header.
 *
 * This struct holds info about a 64-bit program header.
 */
typedef struct phdr64_s {
	uint16_t idx;       /**< Index of the program header. */
	Elf64_Off p_offset;   /**< Program header offset. */
	Elf64_Off mem_offset; /**< Memory offset. */
	elftool_t *bin;       /**< Pointer to elftool structure. */
} phdr64_t;

/**
 * @brief Gets the 64-bit program header entry.
 *
 * @param phdr64 The 64-bit program header.
 */
#define get_phdr64_ent(phdr64)                                                 \
	((Elf64_Phdr *)&(((phdr64_t *)phdr64)->bin)                                \
			  ->mem[((phdr64_t *)phdr64)->p_offset])
/**
 * @brief Gets the memory address of the 64-bit program header.
 *
 * @param phdr64 The 64-bit program header.
 */
#define get_phdr64_mem(phdr64)                                                 \
	((uint8_t *)&(((phdr64_t *)phdr64)->bin)                                   \
			 ->mem[((phdr64_t *)phdr64)->mem_offset])

/**
 * @brief Represents a 32-bit section header.
 *
 * This struct holds info about a 32-bit section header.
 */
typedef struct shdr32_s {
	uint16_t idx;       /**< Index of the section header. */
	Elf32_Off sh_offset;  /**< Section header offset. */
	Elf32_Off mem_offset; /**< Memory offset. */
	elftool_t *bin;       /**< Pointer to elftool structure. */
} shdr32_t;

/**
 * @brief Gets the 32-bit section header entry.
 *
 * @param shdr32 The 32-bit section header.
 */
#define get_shdr32_ent(shdr32)                                                 \
	((Elf32_Shdr *)&(((shdr32_t *)shdr32)->bin)                                \
			  ->mem[((shdr32_t *)shdr32)->sh_offset])
/**
 * @brief Gets the memory address of the 32-bit section header.
 *
 * @param shdr32 The 32-bit section header.
 */
#define get_shdr32_mem(shdr32)                                                 \
	((uint8_t *)&(((shdr32_t *)shdr32)->bin)                                   \
			 ->mem[((shdr32_t *)shdr32)->mem_offset])

/**
 * @brief Represents a 64-bit section header.
 *
 * This struct holds info about a 64-bit section header.
 */
typedef struct shdr64_s {
	uint16_t idx;       /**< Index of the section header. */
	Elf64_Off sh_offset;  /**< Section header offset. */
	Elf64_Off mem_offset; /**< Memory offset. */
	elftool_t *bin;       /**< Pointer to elftool structure. */
} shdr64_t;

/**
 * @brief Gets the 64-bit section header entry.
 *
 * @param shdr64 The 64-bit section header.
 */
#define get_shdr64_ent(shdr64)                                                 \
	((Elf64_Shdr *)&(((shdr64_t *)shdr64)->bin)                                \
			  ->mem[((shdr64_t *)shdr64)->sh_offset])
/**
 * @brief Gets the memory address of the 64-bit section header.
 *
 * @param shdr64 The 64-bit section header.
 */
#define get_shdr64_mem(shdr64)                                                 \
	((uint8_t *)&(((shdr64_t *)shdr64)->bin)                                   \
			 ->mem[((shdr64_t *)shdr64)->mem_offset])

/**
 * @brief Main structure for the elftool.
 *
 * This structure holds all the information about an ELF file.
 */
typedef struct elftool_s {
	char *path;         /**< Path to the ELF file. */
	size_t length;      /**< Length of the ELF file. */
	uint8_t *mem;       /**< Pointer to the memory-mapped ELF file. */
	uint8_t elfclass;   /**< ELF class (32 or 64-bit). */
	uint8_t endian;     /**< Endianness of the ELF file. */
	list_t *phdr;       /**< List of program headers. */
	list_t *shdr;       /**< List of section headers. */
	list_t *syms;       /**< List of symbols. */
	shdr32_t *shstrtab32; /**< 32-bit section header string table. */
	shdr64_t *shstrtab64; /**< 64-bit section header string table. */
	shdr32_t *strtab32;   /**< 32-bit string table. */
	shdr64_t *strtab64;   /**< 64-bit string table. */
} elftool_t;

/**
 * @brief Gets the 64-bit ELF header.
 *
 * @param bin The elftool structure.
 */
#define get_ehdr64(bin) ((Elf64_Ehdr *)(bin)->mem)
/**
 * @brief Gets the 32-bit ELF header.
 *
 * @param bin The elftool structure.
 */
#define get_ehdr32(bin) ((Elf32_Ehdr *)(bin)->mem)

#endif
