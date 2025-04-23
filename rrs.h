#ifndef RRS_H
#define RRS_H

#include "Queue.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    Queue *readyQueue;
    Queue *blockedQueue[3];
    int timeQuantum;
    int currentQuantum;
    bool currentlyRunning;
} RR_Scheduler;

RR_Scheduler *RRSCreate(int timeQuantum);

void RRSInsertTask(RR_Scheduler *scheduler, PCB pcb);

void RRSRemoveTask(RR_Scheduler *scheduler);

void RRSStart(RR_Scheduler *scheduler);

void RRSEnd(RR_Scheduler *scheduler);

void RRSBlock(RR_Scheduler *scheduler);

void RRSStep(RR_Scheduler *scheduler);

#endif
