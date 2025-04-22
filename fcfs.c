#include "fcfs.h"
#include "Queue.h"
// #include "whateverFileWillContainTheCommands"

FCFS_Scheduler *FCFSCreate()
{
    FCFS_Scheduler *s = malloc(sizeof(FCFS_Scheduler));
    s->readyQueue = CreateQueue();
    s->currentlyRunning = false;
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

void FCFSStart(FCFS_Scheduler *scheduler)
{
    if (!scheduler->readyQueue->size)
        return;

    while (scheduler->currentlyRunning && scheduler->readyQueue->first->pcb->programCounter != scheduler->readyQueue->first->pcb->memEnd)
    {
        // run_line(scheduler->readyQueue->first->pcb)
        scheduler->readyQueue->first->pcb->programCounter++;
    }

    if (scheduler->readyQueue->first->pcb->programCounter == scheduler->readyQueue->first->pcb->memEnd)
    {
        // finished
        FCFSRemoveTask(scheduler);
        return;
    }
    return;
}

void FCFSStop(FCFS_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
    return;
}
