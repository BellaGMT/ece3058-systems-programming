#include "paging.h"
#include "page_splitting.h"
#include "swapops.h"
#include "stats.h"

 /* Frame and page table management routines. */
fte_t *frame_table;

/* Frame and page table management routines. */
void system_init(void) {
    /*
     * 1. Set the frame table pointer to point to the first frame in physical
     * memory. Zero out the memory used by the frame table.
     *
     * Address "0" in memory will be used for the frame table. This table will
     * contain n frame table entries (fte_t), where n is the number of
     * frames in memory. The frame table will be useful later if we need to
     * evict pages during page faults.
     */

    frame_table = (fte_t *)mem;

    memset(frame_table, 0, PAGE_SIZE);

    /*for (pfn_t i = 0; i < NUM_FRAMES; ++i) {
        frame_table[i].protected = 0;
        frame_table[i].mapped = 0;
        frame_table[i].referenced = 0;
        frame_table[i].process = NULL;
        frame_table[i].vpn = 0;
    } */

    /*
     * 2. Mark the first frame table entry as protected.
     *
     * The frame table contains entries for all of physical memory,
     * however, there are some frames we never want to evict.
     * We mark these special pages as "protected" to indicate this.
     */

    frame_table[0].protected = 1;
}

/* Frame and page table management routines. */
void proc_init(pcb_t *proc) {
    /* Frame and page table management routines. */

    pfn_t pt_frame = free_frame();
    memset(mem + pt_frame * (PAGE_SIZE), 0, PAGE_SIZE);


    /* Frame and page table management routines. */

    proc->saved_ptbr = pt_frame; 

    frame_table[pt_frame].protected = 1;
	
	frame_table[pt_frame].process = proc;


}

/* Frame and page table management routines. */
void context_switch(pcb_t *proc) {
    PTBR = proc->saved_ptbr;

}

/* Frame and page table management routines. */
uint8_t mem_access(vaddr_t address, char rw, uint8_t data) {

    //stats.accesses++;
    /*if (rw == 'r') {
        data = 0;
    }*/

    /* Split the address and find the page table entry */
    uint16_t vOffset = vaddr_offset(address);
    vpn_t vpn = vaddr_vpn(address);

    pte_t* pgtabentry = (pte_t*) (mem + (PTBR * PAGE_SIZE)) + vpn;

    
    /* If an entry is invalid, just page fault to allocate a page for the page table. */
    if (pgtabentry->valid == 0) {
        //stats.page_faults++;
        page_fault(address);
    }

    /* Set the "referenced" bit to reduce the page's likelihood of eviction */
    frame_table[pgtabentry->pfn].referenced = 1;

    /* Frame and page table management routines. */

   uint8_t* phys_addr = (mem + (pgtabentry->pfn * PAGE_SIZE)) + vOffset;

    /* Either read or write the data to the physical address
       depending on 'rw' */
    if (rw == 'r') {
        stats.reads++;
        //pgtabentry->dirty = 0;
        //printf("m\n");
        //return *phys_addr;
        //printf("o\n");
    } else {
        stats.writes++;
        pgtabentry->dirty = 1;
       // *phys_addr = data;
        memset(phys_addr, data, sizeof(uint8_t));
        /*if (pgtabentry->dirty == 1) {
            stats.writebacks++;
            pgtabentry->dirty = 0;
        }*/
        //printf("p\n");
        //return *phys_addr;
        //printf("n\n");
    }
    //printf("df\n");
    return *phys_addr;
    //printf("pp\n");
}

/* Frame and page table management routines. */
void proc_cleanup(pcb_t *proc) {
    /* Look up the process's page table */
    pte_t* p = (pte_t*) (mem + (PAGE_SIZE * (proc->saved_ptbr)));
    /* Iterate the page table and clean up each valid page */
    for (size_t i = 0; i < NUM_PAGES; i++) {
        pfn_t temp = p[i].pfn;
        if (p[i].valid == 1) {
            frame_table[temp].mapped = 0; //!!!
        }
        if (swap_exists(&p[i])) {
            swap_free(&p[i]);
        }
    }

    /* Free the page table itself in the frame table */
    frame_table[proc->saved_ptbr].protected = 0;
}
