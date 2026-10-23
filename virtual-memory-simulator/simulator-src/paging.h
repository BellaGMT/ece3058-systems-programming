#pragma once

#include "swap.h"
#include "types.h"
#include "pagesim.h"
#include "page_splitting.h"

/*
 * An entry in the page table.
 *
 * A page table maps indices (often referred to as virtual page numbers, or
 * VPNs) to corresponding physical frames in memory. Note that the VPN is not
 * stored in the entry - it's the index into the page table!
 */
typedef struct ptable_entry {
    uint8_t valid;              /* 1 if the entry is mapped to a valid frame, 0
                                   otherwise */
    uint8_t dirty;              /* 1 if the entry has been modified from its
                                   form on disk and must be written back when it
                                   is next evicted. */
    pfn_t pfn;                 /* The physical frame number (PFN) this entry
                                   maps to. */
    swap_entry_t swap;          /* The swap entry mapped to this page. Use this
                                   to read to/write from the page to disk using
                                   swap_read() and swap_write() */
} pte_t;

/* Virtual memory mapping structures and APIs. */
typedef struct ft_entry {
    /* -- Used for page table pages -- */
    uint8_t protected;          /* 1 if the frame holds a page table and is
                                   immune from eviction, 0 otherwise */
    /* -- Used for data pages -- */
    uint8_t mapped;             /* 1 if the frame is mapped, 0
                                   otherwise */
    uint8_t referenced;         /* 1 if the entry has been recently
                                   used, 0 otherwise */
    pcb_t *process;             /* A pointer to the owning process's PCB */
    vpn_t vpn;                  /* The VPN mapped by the process using this frame. */
} fte_t;

/* Virtual memory mapping structures and APIs. */
extern fte_t *frame_table;      /* The frame table */

/* Virtual memory mapping structures and APIs. */
void system_init(void);

void proc_init(pcb_t *proc);
void context_switch(pcb_t *proc);
void proc_cleanup(pcb_t *proc);

uint8_t mem_access(vaddr_t address, char write, uint8_t data);

pfn_t free_frame(void);
void page_fault(vaddr_t address);
