/*
 * student.h
 * Multithreaded OS Simulation for CS 2200 and ECE 3058
 * YOU WILL NOT NEED TO MODIFY THIS FILE
 */
#ifndef __STUDENT_H__
#define __STUDENT_H__
#include "os-sim.h"
extern void idle(unsigned int cpu_id);
extern void preempt(unsigned int cpu_id);
extern void yield(unsigned int cpu_id);
extern void terminate(unsigned int cpu_id);
extern void wake_up(pcb_t *process);
#endif
