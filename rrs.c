#include "rrs.h"
#include "Queue.h"
// #include "whateverFileWillContainTheCommands"

RRS_Scheduler *RRSCreate(int timeQuantum)
{
    RRS_Scheduler *s = malloc(sizeof(RRS_Scheduler));
    s->readyQueue = malloc(sizeof(Queue));
    s->timeQuantum = timeQuantum;
    s->blockedQueue = malloc(sizeof(Queue));
    s->currentQuantum = 0;
    return s;
}

void RRSInsertTask(RRS_Scheduler *scheduler, PCB pcb)
{
    InsertLast(scheduler->readyQueue, pcb);
}

void FCFSRemoveTask(RRS_Scheduler *scheduler)
{
    RemoveFirst(scheduler->readyQueue);
}

void RRSStart(RRS_Scheduler *scheduler)
{
    if (!scheduler->readyQueue->size || !scheduler->currentlyRunning)
        return;

    while (scheduler->currentlyRunning && scheduler->readyQueue->first->pcb->programCounter != scheduler->readyQueue->first->pcb->memEnd && scheduler->currentQuantum < scheduler->timeQuantum)
    {
        // run_line(scheduler->readyQueue->first->pcb)
        scheduler->readyQueue->first->pcb->programCounter++;
        scheduler->currentQuantum++;
    }

    if (scheduler->readyQueue->first->pcb->programCounter == scheduler->readyQueue->first->pcb->memEnd)
    {
        // finished
        RRSRemoveTask(scheduler);
        return;
    }
    else
    {
        // not finished, rotate
        Rotate(scheduler->readyQueue);
    }
    return;
}

void RRSStop(RRS_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
    return;
}

void RRSBlock(RRS_Scheduler *scheduler)
{
    PCB pcb = RemoveFirst(scheduler->readyQueue->first);
    InsertLast(scheduler->readyQueue, pcb);
}
