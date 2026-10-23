/* Process scheduling and synchronization routines. */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "os-sim.h"

/** Function prototypes **/
extern void idle(unsigned int cpu_id);
extern void preempt(unsigned int cpu_id);
extern void yield(unsigned int cpu_id);
extern void terminate(unsigned int cpu_id);
extern void wake_up(pcb_t *process);


/* Process scheduling and synchronization routines. */
static pcb_t **current;
static pthread_mutex_t current_mutex;
static pthread_mutex_t q_mutex;
static pthread_cond_t ready_cond; 
static char sched; 
static int time_splice;

typedef struct Node {
    pcb_t* curr_process;
    struct Node* next;
} Node;

static int size;
static Node* ready_queue; 

static void enqueue(pcb_t *process)
{
    pthread_mutex_lock(&q_mutex);
   // Node *temp_node = (Node*)malloc(sizeof(Node));

    if (size == 0) {
        ready_queue = (Node*)malloc(sizeof(Node));
        ready_queue->curr_process = process;
        ready_queue->next = NULL;
    }else {
        Node *temp = ready_queue;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        Node *curr = temp->next;
        curr = (Node*)malloc(sizeof(Node));
        curr->curr_process = process;
        curr->next = NULL;
        temp->next = curr;

    }
    size++;
    pthread_cond_signal(&ready_cond);
    pthread_mutex_unlock(&q_mutex);
}

static pcb_t* dequeue() 
{
    pthread_mutex_lock(&q_mutex);
    pcb_t *processes = ready_queue->curr_process;
    if (size == 0) {
        pthread_mutex_unlock(&q_mutex);
        return NULL;
    } else if (size == 1) {
        free(ready_queue);
    } else {
        Node *newNode = ready_queue->next;
        free(ready_queue);
        ready_queue = newNode;
    }
    size--;
    pthread_mutex_unlock(&q_mutex);
    return processes;
}

/* Process scheduling and synchronization routines. */
static void schedule(unsigned int cpu_id)
{
    /* FIX ME */
    pthread_mutex_lock(&current_mutex);
    pcb_t* remove = dequeue();

    if (remove == NULL) {
        context_switch(cpu_id, NULL, time_splice);
    } else {
        remove->state = PROCESS_RUNNING;
        current[cpu_id] = remove;
        context_switch(cpu_id, remove, time_splice);
    }

    pthread_mutex_unlock(&current_mutex);
}


/* Process scheduling and synchronization routines. */
extern void idle(unsigned int cpu_id)
{
    /* FIX ME */
    /* Process scheduling and synchronization routines. */
    //mt_safe_usleep(1000000);

    pthread_mutex_lock(&current_mutex);

    if (size == 0) {
        pthread_cond_wait(&ready_cond, &current_mutex);
    }

    pthread_mutex_unlock(&current_mutex);
    schedule(cpu_id);
}


/*
 * preempt() is the handler called by the simulator when a process is
 * preempted due to its timeslice expiring.
 *
 * This function should place the currently running process back in the
 * ready queue, and call schedule() to select a new runnable process.
 */
extern void preempt(unsigned int cpu_id)
{
    /* FIX ME */
    pthread_mutex_lock(&current_mutex);
    pcb_t* curr = current[cpu_id];

    curr->state = PROCESS_READY;
    enqueue(curr);

    pthread_mutex_unlock(&current_mutex);
    pthread_cond_signal(&ready_cond);
    schedule(cpu_id);
}


/*
 * yield() is the handler called by the simulator when a process yields the
 * CPU to perform an I/O request.
 *
 * It should mark the process as WAITING, then call schedule() to select
 * a new process for the CPU.
 */
extern void yield(unsigned int cpu_id)
{
    /* FIX ME */
    pthread_mutex_lock(&current_mutex);

    current[cpu_id]->state = PROCESS_WAITING;
    pthread_mutex_unlock(&current_mutex);

    schedule(cpu_id);
}


/*
 * terminate() is the handler called by the simulator when a process completes.
 * It should mark the process as terminated, then call schedule() to select
 * a new process for the CPU.
 */
extern void terminate(unsigned int cpu_id)
{
    /* FIX ME */
    pthread_mutex_lock(&current_mutex);

    current[cpu_id]->state = PROCESS_TERMINATED;
    pthread_mutex_unlock(&current_mutex);

    schedule(cpu_id);
}


/* Process scheduling and synchronization routines. */
extern void wake_up(pcb_t *process)
{
    /* FIX ME */
    pthread_mutex_lock(&current_mutex);

    process->state = PROCESS_READY;
    enqueue(process);

    pthread_mutex_unlock(&current_mutex);
    
}


/* Process scheduling and synchronization routines. */
int main(int argc, char *argv[])
{
    unsigned int cpu_count;

    /* Parse command-line arguments */
    if (argc > 4)
    {
        fprintf(stderr, "Multithreaded OS Simulator\n"
            "Usage: ./os-sim <# CPUs> [ -l | -r <time slice> ]\n"
            "    Default : FIFO Scheduler\n"
	    "         -l : Longest Remaining Time First Scheduler\n"
            "         -r : Round-Robin Scheduler\n\n");
        return -1;
    }
    cpu_count = strtoul(argv[1], NULL, 0);

    if (argv[2] != NULL && argv[3] != NULL)  {
        sched = 'r';
        time_splice = atoi(argv[3]);
    } else {
        sched = 'f';
        time_splice = -1;
    }

    /* FIX ME - Add support for -l and -r parameters*/
    ready_queue = NULL;
    size = 0;

    /* Allocate the current[] array and its mutex */
    current = malloc(sizeof(pcb_t*) * cpu_count);
    assert(current != NULL);

    pthread_mutex_init(&current_mutex, NULL);
    pthread_mutex_init(&q_mutex, NULL);
    pthread_cond_init(&ready_cond, NULL);
    

    /* Start the simulator in the library */
    start_simulator(cpu_count);

    return 0;
}


