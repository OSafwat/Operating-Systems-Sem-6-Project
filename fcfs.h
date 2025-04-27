#ifndef FCFS_H
#define FCFS_H

#include "Queue.h"
#include <stdio.h>
#include <stdbool.h>
#include "MileStone2.h"

typedef struct
{
    Queue *readyQueue;
    bool currentlyRunning;
} FCFS_Scheduler;

FCFS_Scheduler *FCFSCreate(void);

void FCFSInsertTask(FCFS_Scheduler *scheduler, PCB pcb);

void FCFSRemoveTask(FCFS_Scheduler *scheduler);

void FCFSStart(FCFS_Scheduler *scheduler);

void FCFSStop(FCFS_Scheduler *scheduler);

void FCFSStep(FCFS_Scheduler *scheduler);

#endif
