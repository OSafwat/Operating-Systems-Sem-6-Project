#include "mlfqs.h"
#include "queue.h"
#include <stdlib.h>
// #include "whateverFileWillContainTheCommands"

MLFQS_Scheduler *MLFQSCreate()
{
    MLFQS_Scheduler *s = malloc(sizeof(MLFQS_Scheduler));
    for (int i = 0; i < 4; i++)
    {
        s->readyQueue[i] = CreateQueue();
        if (i != 3)
            s->blockedQueue[i] = CreateQueue();
        s->timeQuantum[i] = (1 << i);
        s->currentQuantum[i] = 0;
    }
    s->currentlyRunning = true;
    return s;
}

void MLFQSInsertTask(MLFQS_Scheduler *scheduler, PCB pcb)
{
    pcb.priority = 1;
    InsertLast(scheduler->readyQueue[0], pcb);
}

void MLFQSRemoveTask(MLFQS_Scheduler *scheduler, int index)
{
    RemoveFirst(scheduler->readyQueue[index]);
}

void MLFQSStep(MLFQS_Scheduler *scheduler)
{
    bool isEmpty = true;
    for (int i = 0; i < 4; i++)
        isEmpty &= !scheduler->readyQueue[i]->size;
    if (isEmpty || !scheduler->currentlyRunning)
        return;
    bool opDone = false;
    for (int i = 0; i < 4; i++)
    {
        while (scheduler->readyQueue[i]->size)
        {
            bool exit = false;
            int res = 0;
            while (scheduler->currentlyRunning && scheduler->readyQueue[i]->size && scheduler->readyQueue[i]->first->pcb->programCounter != scheduler->readyQueue[i]->first->pcb->memEnd + 1 && scheduler->currentQuantum[i] < scheduler->timeQuantum[i])
            {
                opDone = true;
                res = Parse(scheduler->readyQueue[i]->first->pcb);
                scheduler->readyQueue[i]->first->pcb->programCounter++;
                if (res > 0)
                {
                    // should be blocked
                    MLFQSBlock(scheduler, res, i);
                    scheduler->currentQuantum[i] = 0;
                }
                if (res < 0)
                {
                    // should be unblocked
                    MLFQSFree(scheduler, -res);
                    exit = true;
                }
                scheduler->currentQuantum[i]++;
                if (exit)
                    break;
                break;
            }
            if (scheduler->readyQueue[i]->size && scheduler->readyQueue[i]->first->pcb->programCounter == scheduler->readyQueue[i]->first->pcb->memEnd + 1)
            {
                // done, remove
                MLFQSRemoveTask(scheduler, i);
                scheduler->currentQuantum[i] = 0;
            }
            else
            {
                // not done, rotate if 4th, move down if not 4th AND THE REST IS NOT EMPTY, if it is the only task in the queue, then it remains on top
                if (i == 3 && scheduler->currentQuantum[i] == scheduler->timeQuantum[i])
                {
                    // 4th, rotate
                    // MakeReady(scheduler->readyQueue[i]->first->pcb)
                    InsertLast(scheduler->readyQueue[i], RemoveFirst(scheduler->readyQueue[i]));
                }
                else if (scheduler->currentQuantum[i] == scheduler->timeQuantum[i] && (i != 0 || ((scheduler->readyQueue[0] + scheduler->readyQueue[1]->size + scheduler->readyQueue[2]->size + scheduler->readyQueue[3]->size) && scheduler->readyQueue[i]->size)))
                {
                    // either not the first, or it is the first but everything else is non empty, so either way push down
                    // MakeReady(scheduler->readyQueue[i]->first->pcb)
                    PCB pcb = RemoveFirst(scheduler->readyQueue[i]);
                    pcb.priority++;
                    InsertLast(scheduler->readyQueue[i + 1], pcb);
                }
                if (!exit)
                    scheduler->currentQuantum[i] = 0;
            }
            if (exit)
            {
                i = -1;
                break;
            }
            if (opDone)
                return;
        }
    }
    return;
}

void MLFQSStart(MLFQS_Scheduler *scheduler)
{
    bool isEmpty = true;
    for (int i = 0; i < 4; i++)
        isEmpty &= !scheduler->readyQueue[i]->size;
    if (isEmpty || !scheduler->currentlyRunning)
        return;
    for (int i = 0; i < 4; i++)
    {
        while (scheduler->readyQueue[i]->size)
        {
            bool exit = false;
            int res = 0;
            while (scheduler->currentlyRunning && scheduler->readyQueue[i]->size && scheduler->readyQueue[i]->first->pcb->programCounter != scheduler->readyQueue[i]->first->pcb->memEnd + 1 && scheduler->currentQuantum[i] < scheduler->timeQuantum[i])
            {
                res = Parse(scheduler->readyQueue[i]->first->pcb);
                scheduler->readyQueue[i]->first->pcb->programCounter++;
                if (res > 0)
                {
                    // should be blocked
                    MLFQSBlock(scheduler, res, i);
                    scheduler->currentQuantum[i] = 0;
                }
                if (res < 0)
                {
                    // should be unblocked
                    MLFQSFree(scheduler, -res);
                    exit = true;
                }
                scheduler->currentQuantum[i]++;
                if (exit)
                    break;
            }
            if (scheduler->readyQueue[i]->size && scheduler->readyQueue[i]->first->pcb->programCounter == scheduler->readyQueue[i]->first->pcb->memEnd + 1)
            {
                // done, remove
                MLFQSRemoveTask(scheduler, i);
                scheduler->currentQuantum[i] = 0;
            }
            else
            {
                // not done, rotate if 4th, move down if not 4th AND THE REST IS NOT EMPTY, if it is the only task in the queue, then it remains on top
                if (i == 3 && scheduler->currentQuantum[i] == scheduler->timeQuantum[i])
                {
                    // 4th, rotate
                    // MakeReady(scheduler->readyQueue[i]->first->pcb)
                    InsertLast(scheduler->readyQueue[i], RemoveFirst(scheduler->readyQueue[i]));
                }
                else if (scheduler->currentQuantum[i] == scheduler->timeQuantum[i] && (i != 0 || ((scheduler->readyQueue[0] + scheduler->readyQueue[1]->size + scheduler->readyQueue[2]->size + scheduler->readyQueue[3]->size) && scheduler->readyQueue[i]->size)))
                {
                    // either not the first, or it is the first but everything else is non empty, so either way push down
                    // MakeReady(scheduler->readyQueue[i]->first->pcb)
                    PCB pcb = RemoveFirst(scheduler->readyQueue[i]);
                    pcb.priority++;
                    InsertLast(scheduler->readyQueue[i + 1], pcb);
                }
                if (!exit)
                    scheduler->currentQuantum[i] = 0;
            }
            if (exit)
            {
                i = -1;
                break;
            }
        }
    }
    return;
}

void MLFQSStop(MLFQS_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
}

void MLFQSBlock(MLFQS_Scheduler *scheduler, int resourceIndex, int taskIndex)
{
    if (resourceIndex == 1)
        modifyFileBlocked(1);
    else if (resourceIndex == 2)
        modifyInputBlocked(1);
    else
        modifyOutputBlocked(1);
    resourceIndex--;
    PCB pcb = RemoveFirst(scheduler->readyQueue[taskIndex]);
    SetPriority(&pcb, taskIndex);
    PriorityInsert(scheduler->blockedQueue[resourceIndex], pcb);
}

void MLFQSFree(MLFQS_Scheduler *scheduler, int resourceIndex)
{
    if (resourceIndex == 1)
        modifyFileBlocked(-1);
    else if (resourceIndex == 2)
        modifyInputBlocked(-1);
    else
        modifyOutputBlocked(-1);
    // whatever was blocked should be replaced
    resourceIndex--;
    if (!scheduler->blockedQueue[resourceIndex]->size)
        return;
    PCB pcb = RemoveFirst(scheduler->blockedQueue[resourceIndex]);
    MakeReady(&pcb);
    InsertLast(scheduler->readyQueue[pcb.priority], pcb);
}
