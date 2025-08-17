#ifndef ELFTOOL_QUERY_H
#define ELFTOOL_QUERY_H

#include "elftool_structs.h"

typedef enum op_type {
    ELFTOOL_QUERY_READ,
    ELFTOOL_QUERY_WRITE,
} op_type_t;

typedef struct elftool_sym_query_s {
    op_type_t op;       /**< Operation type: read, write */
    void *sym_content;  /**< Symbol content, returned by read op, or written by
                           write op. */
    size_t sym_size;    /**< Size of symbol content. */
    uint64_t sym_vaddr; /**< Virtual address of symbol. */
} elftool_sym_query_t;

/*
 * @brief execute query to symbol "name"
 *
 * Search through symbol list for a symbol matching @param name.
 * Apply the query on it (ELFTOOL_QUERY_READ or ELFTOOL_QUERY_WRITE)
 * */
int elftool_sym_query_by_name(elftool_t *bin, elftool_sym_query_t *query,
                              char *name);

#endif
