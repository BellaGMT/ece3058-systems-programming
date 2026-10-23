/**
 * @author ECE 3058 TAs
 */
#ifndef __CACHESIM_H
#define __CACHESIM_H
#define MEMREAD 0
#define MEMWRITE 1
#define IFETCH 2
#include "lrustack.h"
typedef unsigned long long addr_t;
typedef unsigned long long counter_t;
typedef struct cache_block_t {
 int tag;
 int valid;
 int dirty;
} cache_block_t;
typedef struct cache_set_t {
 int size;
 lru_stack_t* stack;
 cache_block_t* blocks;
} cache_set_t;
void cachesim_init(int block_size, int cache_size, int ways);
void cachesim_access(addr_t physical_add, int access_type);
void cachesim_cleanup(void);
void cachesim_print_stats(void);
#endif
