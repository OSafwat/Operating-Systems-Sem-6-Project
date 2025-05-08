#include "rrs.h"
#include "Queue.h"
#include <stdlib.h>
// #include "whateverFileWillContainTheCommands"

RR_Scheduler *RRSCreate(int timeQuantum)
{
    RR_Scheduler *s = malloc(sizeof(RR_Scheduler));
    s->readyQueue = CreateQueue();
    s->timeQuantum = timeQuantum;
    for (int i = 0; i < 3; i++)
        s->blockedQueue[i] = CreateQueue();
    s->currentQuantum = 0;
    s->currentlyRunning = true;
    return s;
}

void RRSInsertTask(RR_Scheduler *scheduler, PCB pcb)
{
    InsertLast(scheduler->readyQueue, pcb);
}

void RRSRemoveTask(RR_Scheduler *scheduler)
{
    RemoveFirst(scheduler->readyQueue);
}

void RRSStep(RR_Scheduler *scheduler)
{
    if (!scheduler->readyQueue->size || !scheduler->currentlyRunning)
        return;
    int res = Parse(scheduler->readyQueue->first->pcb);
    if (res > 0)
    {
        RRSBlock(scheduler, res);
        scheduler->currentQuantum = 0;
        return;
    }
    scheduler->readyQueue->first->pcb->programCounter++;
    scheduler->currentQuantum++;
    if (res < 0)
    {
        RRSFree(scheduler, -res);
    }
    if (scheduler->readyQueue->first->pcb->programCounter == scheduler->readyQueue->first->pcb->memEnd)
    {
        RRSRemoveTask(scheduler);
        scheduler->currentQuantum = 0;
        return;
    }
    if (scheduler->currentQuantum == scheduler->timeQuantum)
    {
        InsertLast(scheduler->readyQueue, RemoveFirst(scheduler->readyQueue));
        scheduler->currentQuantum = 0;
        return;
    }
}

void RRSStart(RR_Scheduler *scheduler)
{
    if (!scheduler->readyQueue->size || !scheduler->currentlyRunning)
        return;
    while (scheduler->currentlyRunning && scheduler->readyQueue->size)
    {
        int res = 0;
        while (scheduler->readyQueue->size && scheduler->currentlyRunning && scheduler->readyQueue->first->pcb->programCounter != scheduler->readyQueue->first->pcb->memEnd + 1 && scheduler->currentQuantum < scheduler->timeQuantum)
        {
            res = Parse(scheduler->readyQueue->first->pcb);
            scheduler->readyQueue->first->pcb->programCounter++;
            scheduler->currentQuantum++;
            if (res > 0)
            {
                // blocked
                RRSBlock(scheduler, res);
                scheduler->currentQuantum = 0;
                res = 0;
                continue;
            }
            if (res < 0)
            {
                // freed
                RRSFree(scheduler, -res);
                res = 0;
            }
        }
        scheduler->currentQuantum = 0;

        // done, remove
        if (scheduler->readyQueue->first->pcb->programCounter == scheduler->readyQueue->first->pcb->memEnd + 1)
        {
            RRSRemoveTask(scheduler);
            continue;
        }

        // all tasks are done
        if (!scheduler->readyQueue->size)
            return;

        // not finished, rotate and make it ready
        MakeReady(scheduler->readyQueue->first->pcb);
        InsertLast(scheduler->readyQueue, RemoveFirst(scheduler->readyQueue));
    }
    return;
}

void RRSStop(RR_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
    return;
}

void RRSBlock(RR_Scheduler *scheduler, int resourceIndex)
{
    if (resourceIndex == 1)
        modifyFileBlocked(1);
    else if (resourceIndex == 2)
        modifyInputBlocked(1);
    else
        modifyOutputBlocked(1);
    resourceIndex--;
    PCB pcb = RemoveFirst(scheduler->readyQueue);
    InsertLast(scheduler->blockedQueue[resourceIndex], pcb);
}

void RRSFree(RR_Scheduler *scheduler, int resourceIndex)
{
    if (resourceIndex == 1)
        modifyFileBlocked(-1);
    else if (resourceIndex == 2)
        modifyInputBlocked(-1);
    else
        modifyOutputBlocked(-1);
    resourceIndex--;
    if (!scheduler->blockedQueue[resourceIndex]->size)
        return;
    PCB pcb = RemoveFirst(scheduler->blockedQueue[resourceIndex]);
    MakeReady(&pcb);
    InsertLast(scheduler->readyQueue, pcb);
}
