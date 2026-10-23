#include "types.h"
#include "pagesim.h"
#include "paging.h"
#include "swapops.h"
#include "stats.h"
#include "util.h"

pfn_t select_victim_frame(void);

pfn_t free_frame(void) {
    pfn_t victim_pfn = select_victim_frame();

    if (frame_table[victim_pfn].mapped == 1) {
        pfn_t temp = frame_table[victim_pfn].process->saved_ptbr;
        pte_t* p = (pte_t*) (mem + (temp * PAGE_SIZE));
        pte_t* pgtabentry = p + frame_table[victim_pfn].vpn;
        if (pgtabentry->dirty == 1) {
            uint8_t* f = mem + victim_pfn * PAGE_SIZE;
            stats.writebacks++;
            swap_write(pgtabentry, f);
            pgtabentry->dirty = 0;
        }
        pgtabentry->valid = 0;
        frame_table[victim_pfn].mapped = 0;
    }

    return victim_pfn;
}

pfn_t select_victim_frame() {
    size_t num_entries = MEM_SIZE / PAGE_SIZE;
    for (size_t i = 0; i < num_entries; i++) {
        if (!frame_table[i].protected && !frame_table[i].mapped) {
            return i;
        }
    }

    if (replacement == RANDOM) {
        pfn_t last_unprotected = NUM_FRAMES;
        for (pfn_t i = 0; i < num_entries; i++) {
            if (!frame_table[i].protected) {
                last_unprotected = i;
                if (prng_rand() % 2) {
                    return i;
                }
            }
        }
        if (last_unprotected < NUM_FRAMES) {
            return last_unprotected;
        }
    }

    panic("System ran out of memory\n");
    exit(1);
}
