#include "paging.h"
#include "swapops.h"
#include "stats.h"

/* Resolve page faults by allocating frames and restoring page contents. */
void page_fault(vaddr_t address) {

   stats.page_faults++;

    /* First, split the faulting address and locate the page table entry */

   vpn_t vpn = vaddr_vpn(address);
   pte_t* pgtabentry = (pte_t*) (mem + (PTBR * PAGE_SIZE)) + vpn;

    /* It's a page fault, so the entry obviously won't be valid. Grab
       a frame to use by calling free_frame(). */

   pfn_t f_frame = free_frame();

    /* Resolve page faults by allocating frames and restoring page contents. */

   pgtabentry->valid = 1;
   pgtabentry->dirty = 0;
   pgtabentry->pfn = f_frame;

    /* Resolve page faults by allocating frames and restoring page contents. */
   
   frame_table[pgtabentry->pfn].mapped = 1;
   frame_table[pgtabentry->pfn].process= current_process;
   frame_table[pgtabentry->pfn].vpn = vpn;


    /* Initialize the page's memory. On a page fault, it is not enough
     * just to allocate a new frame. We must load in the old data from
     * disk into the frame. If there was no old data on disk, then
     * we need to clear out the memory (why?).
     *
     * 1) Get a pointer to the new frame in memory.
     * 2) If the page has swap set, then we need to load in data from memory
     *    using swap_read().
     * 3) Else, just clear the memory.
     *
     * Otherwise, zero the page's memory. If the page is later written
     * back, swap_write() will automatically allocate a swap entry.
     */

    uint8_t* frames = mem + (f_frame * PAGE_SIZE);
    if (swap_exists(pgtabentry)) {
      swap_read(pgtabentry, frames);
    }
    else {
      memset(frames, 0, PAGE_SIZE);
    }

}
