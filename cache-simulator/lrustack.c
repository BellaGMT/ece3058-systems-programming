/**
 * @author ECE 3058 TAs
 */

#include <stdlib.h>
#include "lrustack.h"

/* Least-recently-used cache replacement operations. */

/**
 * Function to initialize an LRU stack for a cache set with a given <size>. This function
 * creates the LRU stack. 
 * 
 * @param size is the size of the LRU stack to initialize. 
 * @return the dynamically allocated stack. 
 */
lru_stack_t* init_lru_stack(int size) {
    //  Use malloc to dynamically allocate a lru_stack_t
	lru_stack_t* stack = (lru_stack_t*) malloc(sizeof(lru_stack_t));
    //  Set the stack size the caller passed in
	stack->size = size;
    
    ////////////////////////////////////////////////////////////////////
    //  added to lru_stack_t.
    ////////////////////////////////////////////////////////////////////
    stack->stack_array = (int*) malloc(size * sizeof(int));
    int i = 0;
    while (i < size) {
        stack->stack_array[i] = -1; 
        i++;
    }
    stack->top = size - 1;  

	return stack;
}

/* Least-recently-used cache replacement operations. */
int lru_stack_get_lru(lru_stack_t* stack) {
    ////////////////////////////////////////////////////////////////////
    //  Stack. 
    ////////////////////////////////////////////////////////////////////

    return stack->stack_array[0];
}

/**
 * Function to mark the cache block with index <n> as MRU in <stack>. This operation should
 * change/mutate the LRU stack.
 * 
 * @param stack is the stack to run the operation on.
 * @param n the index to promote to MRU.  
 */
void lru_stack_set_mru(lru_stack_t* stack, int n) {
	////////////////////////////////////////////////////////////////////
    //  element in the LRU Stack. 
    ////////////////////////////////////////////////////////////////////

    for (int i = 0; i < stack->size - 1; i++) {
        stack->stack_array[i] = stack->stack_array[i + 1];
    }
    stack->stack_array[stack->size - 1] = n;
    
}

/* Least-recently-used cache replacement operations. */
void lru_stack_cleanup(lru_stack_t* stack) {
    ////////////////////////////////////////////////////////////////////
    //  cleanup
    ////////////////////////////////////////////////////////////////////
    free(stack->stack_array);
     
    free(stack);        // Free the stack struct we malloc'd
}