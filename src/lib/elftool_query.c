#include "elftool_query.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <errno.h>

int elftool_sym_query_by_name(elftool_t *bin, elftool_sym_query_t *query, char *name)
{
    _Bool found = false;
    size_t sym_size = 0;
    uint8_t* sym_content = 0;
    Elf64_Addr sym_addr = 0;

    for (list_t *head = bin->syms; head; head = head->next) {
        if (!strncmp(name, ((sym64_t*)head->content)->name, strlen(name))) {
            if (get_sym64_ent(head->content)->st_value != 0
                    && get_sym64_ent(head->content)->st_size != 0) {
                // printf("Found symbol [%s]\n", name);
                sym_size = get_sym64_ent(head->content)->st_size;
                sym_addr = get_sym64_ent(head->content)->st_value;
                for (list_t *head = bin->phdr; head; head = head->next) {
                    if (get_phdr64_ent(head->content)->p_type == PT_LOAD
                            && (sym_addr > get_phdr64_ent(head->content)->p_vaddr
                                && sym_addr < get_phdr64_ent(head->content)->p_vaddr +
                                    get_phdr64_ent(head->content)->p_memsz)) {
                        Elf64_Addr offset_from_segment_start =
                            sym_addr - get_phdr64_ent(head->content)->p_vaddr;
                        if (offset_from_segment_start > get_phdr64_ent(head->content)->p_offset +
                                get_phdr64_ent(head->content)->p_filesz) {
                            fprintf(stderr, "Error symbol is in unitialized part of PT_LOAD segment.");
                            return EINVAL;
                        }
                        Elf64_Addr symbol_content_offset = get_phdr64_ent(head->content)->p_offset +
                            offset_from_segment_start;
                        // printf("found pointer to symbol mem at %lu\n", symbol_content_offset);
                        sym_content = &bin->mem[symbol_content_offset];
                        found = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    if (!found) {
        fprintf(stderr, "Symbol not found\n");
        return EINVAL;
    }
    if (query->op == ELFTOOL_QUERY_READ) {
        query->sym_size = sym_size;
        query->sym_content = sym_content;
    }
    else {
        if (query->sym_size != sym_size) {
            fprintf(stderr, "Error query->sym_size different from symbol size.\n");
            return EINVAL;
        }
        memcpy(sym_content, query->sym_content, sym_size);
    }
    query->sym_vaddr = sym_addr;
    return (0);
}
