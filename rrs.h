#ifndef RRS_H
#define RRS_H

#include "queue.h"
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

int Parse(PCB *CurrentProcess);
void MakeReady(PCB *pcb);
int modifyInputBlocked(int x);
int modifyOutputBlocked(int x);
int modifyFileBlocked(int x);

RR_Scheduler *RRSCreate(int timeQuantum);

void RRSInsertTask(RR_Scheduler *scheduler, PCB pcb);

void RRSRemoveTask(RR_Scheduler *scheduler);

void RRSStart(RR_Scheduler *scheduler);

void RRSStop(RR_Scheduler *scheduler);

void RRSBlock(RR_Scheduler *scheduler, int resourceIndex);

void RRSStep(RR_Scheduler *scheduler);

void RRSFree(RR_Scheduler *scheduler, int resourceIndex);

#endif
