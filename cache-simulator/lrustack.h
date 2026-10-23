/**
 * @author ECE 3058 TAs
 */

#ifndef __LRUSTACK_H
#define __LRUSTACK_H

/* LRU stack interface and related state. */

/* LRU stack interface and related state. */
typedef struct lru_stack_t {
	int size;   // Corresponds to the associativity
    int *stack_array;
    int top;
} lru_stack_t;

/**
 * Function to initialize an LRU stack for a cache set with a given <size>. This function
 * creates the LRU stack. 
 * 
 * @param size is the size of the LRU stack to initialize. 
 * @return the dynamically allocated stack. 
 */
lru_stack_t* init_lru_stack(int size);

/* LRU stack interface and related state. */
int lru_stack_get_lru(lru_stack_t* stack);

/**
 * Function to mark the cache block with index <n> as MRU in <stack>. This operation should
 * change/mutate the LRU stack.
 * 
 * @param stack is the stack to run the operation on.
 * @param n the index to promote to MRU.  
 */
void lru_stack_set_mru(lru_stack_t* stack, int n);

/* LRU stack interface and related state. */
void lru_stack_cleanup(lru_stack_t* stack);

#endif