#pragma once

#include "paging.h"

/* Split virtual addresses into page numbers and offsets. */
static inline vpn_t vaddr_vpn(vaddr_t addr) {
    return addr >> OFFSET_LEN;
}

static inline uint16_t vaddr_offset(vaddr_t addr) {
    return addr & (PAGE_SIZE - 1);
}
