#ifndef MLFQS_H
#define MLFQS_H

#include "queue.h"
#include "PCB.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    Queue *readyQueue[4];
    Queue *blockedQueue[3];
    int timeQuantum[4];
    int currentQuantum[4];
    bool currentlyRunning;
} MLFQS_Scheduler;

void SetPriority(PCB *pcb, int priority);
int Parse(PCB *CurrentProcess);
void MakeReady(PCB *pcb);

MLFQS_Scheduler *MLFQSCreate();

void MLFQSInsertTask(MLFQS_Scheduler *scheduler, PCB pcb);

void MLFQSRemoveTask(MLFQS_Scheduler *scheduler, int index);

void MLFQSStart(MLFQS_Scheduler *scheduler);

void MLFQSEnd(MLFQS_Scheduler *scheduler);

void MLFQSBlock(MLFQS_Scheduler *scheduler, int resourceIndex, int taskIndex);

void MLFQSFree(MLFQS_Scheduler *scheduler, int index);

void MLFQSStop(MLFQS_Scheduler *scheduler);

#endif
