#include "paging.h"
#include "stats.h"

/* The stats. See the definition in stats.h. */
stats_t stats;

/* Memory access statistics and timing calculations. */
void compute_stats() {

    stats.accesses = stats.reads + stats.writes;
    stats.aat = (double) (stats.reads * MEMORY_READ_TIME + stats.writes * MEMORY_READ_TIME + stats.page_faults * DISK_PAGE_READ_TIME + stats.writebacks * DISK_PAGE_WRITE_TIME) / (double) stats.accesses;
    

}
