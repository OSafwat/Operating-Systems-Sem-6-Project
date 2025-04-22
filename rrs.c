#include "rrs.h"
#include "Queue.h"
// #include "whateverFileWillContainTheCommands"

RR_Scheduler *RRSCreate(int timeQuantum)
{
    RR_Scheduler *s = malloc(sizeof(RR_Scheduler));
    s->readyQueue = CreateQueue();
    s->timeQuantum = timeQuantum;
    s->blockedQueue = CreateQueue();
    s->currentQuantum = 0;
    return s;
}

void RRSInsertTask(RR_Scheduler *scheduler, PCB pcb)
{
    InsertLast(scheduler->readyQueue, pcb);
}

void FCFSRemoveTask(RR_Scheduler *scheduler)
{
    RemoveFirst(scheduler->readyQueue);
}

void RRSStart(RR_Scheduler *scheduler)
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
        InsertLast(scheduler->readyQueue, RemoveFirst(scheduler->readyQueue));
    }
    return;
}

void RRSStop(RR_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
    return;
}

void RRSBlock(RR_Scheduler *scheduler)
{
    PCB pcb = RemoveFirst(scheduler->readyQueue->first);
    InsertLast(scheduler->readyQueue, pcb);
}
