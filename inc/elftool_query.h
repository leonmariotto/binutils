#ifndef ELFTOOL_QUERY_H
#define ELFTOOL_QUERY_H

#include "elftool_structs.h"

/**
 * @brief Type of operation for a query.
 */
typedef enum op_type {
	ELFTOOL_QUERY_READ, /**< Read operation. */
	ELFTOOL_QUERY_WRITE, /**< Write operation. */
} op_type_t;

/**
 * @brief Represents a symbol query.
 *
 * This struct holds information about a symbol query.
 */
typedef struct elftool_sym_query_s {
	op_type_t op;      /**< Operation type: read, write */
	void *sym_content; /**< Symbol content for read/write op. */
	size_t sym_size;   /**< Size of symbol content. */
	uint64_t sym_vaddr;/**< Virtual address of symbol. */
} elftool_sym_query_t;

/**
 * @brief Executes a query on a symbol by name.
 *
 * This function searches for a symbol and applies a query.
 *
 * @param bin The elftool structure.
 * @param query The query to apply.
 * @param name The name of the symbol.
 * @return 0 on success, non-zero on failure.
 */
int elftool_sym_query_by_name(elftool_t *bin, elftool_sym_query_t *query,
			      char *name);

#endif
