#ifndef MLFQS_H
#define MLFQS_H

#include "Queue.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    Queue *readyQueue[4];
    Queue *blockedQueue[3];
    int timeQuantum[4];
    int currentQuantum[4];
    bool currentlyRunning;
} MLFQ_Scheduler;

MLFQ_Scheduler *MLFQSSCreate();

void MLFQSInsertTask(MLFQ_Scheduler *scheduler, PCB pcb);

void MLFQSRemoveTask(MLFQ_Scheduler *scheduler, int index);

void MLFQSStart(MLFQ_Scheduler *scheduler);

void MLFQSSEnd(MLFQ_Scheduler *scheduler);

void MLFQSSBlock(MLFQ_Scheduler *scheduler);

void MLFQSFree(MLFQ_Scheduler *scheduler, int index);

#endif
