#ifndef FCFS_H
#define FCFS_H

#include "pcb.h"

typedef struct
{
    PCB pcb;
    char *path;
} Task;

typedef struct
{
    Task *tasks;
    int size;
    int maxCapacity;
} Scheduler;

Scheduler *fcfs_create(int count);

int fcfs_insert_task(Scheduler *sched, PCB pcb, const char *path);

void fcfs_start(Scheduler *sched);

#endif // FCFS_H
