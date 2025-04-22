#ifndef RRS_H
#define RRS_H

#include "Queue.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    Queue *readyQueue;
    Queue *blockedQueue;
    int timeQuantum;
    int currentQuantum;
    bool currentlyRunning;
} RRS_Scheduler;

RRS_Scheduler *RRSCreate(int timeQuantum);

void RRSInsertTask(RRS_Scheduler *scheduler, PCB pcb);

void RRSRemoveTask(RRS_Scheduler *scheduler);

void RRSStart(RRS_Scheduler *scheduler);

void RRSEnd(RRS_Scheduler *scheduler);

void RRSBlock(RRS_Scheduler *scheduler);

#endif
