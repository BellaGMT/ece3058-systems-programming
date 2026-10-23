#pragma once

#include "pagesim.h"
#include "paging.h"
#include "swap.h"
#include "types.h"

extern swap_queue_t swap_queue;

/**
 * Determines if the given page table entry has a swap entry.
 *
 * @param entry a pointer to the page table entry to check
 */
static inline int swap_exists(pte_t *entry) {
    return entry->swap != 0;
}

/* Implementation notes for this component. */
void swap_read(pte_t *entry, void *dst);

/* Implementation notes for this component. */
void swap_write(pte_t *entry, void *src);

/**
 * Frees the swap entry associated with the given page table entry.
 *
 * The space held by the swap entry is de-allocated and the swap field
 * in the page table is automatically cleared.
 *
 * @param entry a pointer to the page table entry
 */
void swap_free(pte_t *entry);
