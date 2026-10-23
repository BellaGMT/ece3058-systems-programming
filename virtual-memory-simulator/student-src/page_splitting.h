#pragma once

#include "paging.h"

/* Virtual address page-number and offset helpers. */

/* Get the virtual page number from a virtual address. */
static inline vpn_t vaddr_vpn(vaddr_t addr) {
    return addr >> OFFSET_LEN;
}

/* Get the offset into the page from a virtual address. */
static inline uint16_t vaddr_offset(vaddr_t addr) {
    return addr & (PAGE_SIZE - 1);
}
