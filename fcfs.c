#include "fcfs.h"
#include "queue.h"
#include <stdlib.h>
// #include "whateverFileWillContainTheCommands"

FCFS_Scheduler *FCFSCreate()
{
    FCFS_Scheduler *s = malloc(sizeof(FCFS_Scheduler));
    s->readyQueue = CreateQueue();
    s->currentlyRunning = true;
    return s;
}

void FCFSInsertTask(FCFS_Scheduler *scheduler, PCB pcb)
{
    InsertLast(scheduler->readyQueue, pcb);
}

void FCFSRemoveTask(FCFS_Scheduler *scheduler)
{
    RemoveFirst(scheduler->readyQueue);
}

void FCFSStep(FCFS_Scheduler *scheduler)
{
    if (!scheduler->readyQueue->size || !scheduler->currentlyRunning)
        return;
    int res = Parse(scheduler->readyQueue->first->pcb);
    scheduler->readyQueue->first->pcb->programCounter++;
    if (scheduler->readyQueue->first->pcb->programCounter == scheduler->readyQueue->first->pcb->memEnd)
    {
        FCFSRemoveTask(scheduler);
    }
}

void FCFSStart(FCFS_Scheduler *scheduler)
{
    if ((scheduler->readyQueue->size == 0) || !scheduler->currentlyRunning)
        return;
    while (scheduler->readyQueue->size)
    {
        while (scheduler->currentlyRunning && scheduler->readyQueue->first->pcb->programCounter != scheduler->readyQueue->first->pcb->memEnd + 1)
        {
            Parse(scheduler->readyQueue->first->pcb);
            scheduler->readyQueue->first->pcb->programCounter++;
        }

        if (scheduler->readyQueue->first->pcb->programCounter == scheduler->readyQueue->first->pcb->memEnd + 1)
        {
            // finished
            FCFSRemoveTask(scheduler);
        }
    }
    return;
}

void FCFSStop(FCFS_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
    return;
}
