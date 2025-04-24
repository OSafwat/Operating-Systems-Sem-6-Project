#include "mlfqs.h"
#include "Queue.h"
// #include "whateverFileWillContainTheCommands"

MLFQ_Scheduler *MLFQSCreate()
{
    MLFQ_Scheduler *s = malloc(sizeof(MLFQ_Scheduler));
    for (int i = 0; i < 4; i++)
    {
        s->readyQueue[i] = CreateQueue();
        if (i != 3)
            s->blockedQueue[i] = CreateQueue();
        s->timeQuantum[i] = (1 << i);
        s->currentQuantum[i] = 0;
    }
    return s;
}

void MLFQSInsertTask(MLFQ_Scheduler *scheduler, PCB pcb)
{
    pcb.priority = 1;
    InsertLast(scheduler->readyQueue[0], pcb);
}

void MLFQSRemoveTask(MLFQ_Scheduler *scheduler, int index)
{
    RemoveFirst(scheduler->readyQueue[index]);
}

void MLFQStart(MLFQ_Scheduler *scheduler)
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
            while (scheduler->currentlyRunning && scheduler->readyQueue[i]->first->pcb->programCounter != scheduler->readyQueue[i]->first->pcb->memEnd && scheduler->currentQuantum[i] < scheduler->timeQuantum[i])
            {
                // res = Parse(scheduler->readyQueue->first->pcb)
                if (res)
                {
                    if (res > 0)
                    {
                        // should be blocked
                        MLFQSBlock(scheduler, res, i);
                        break;
                    }
                    else
                    {
                        // should be unblocked
                        MLFQSFree(scheduler, -res);
                        i = -1;
                        exit = true;
                    }
                }
                scheduler->readyQueue[i]->first->pcb->programCounter++;
                scheduler->currentQuantum[i]++;
                if (exit)
                    break;
            }
            if (!res)
            {
                // blocked, so whatever is currently in the queue (if it is not empty) should just be started
                continue;
            }
            if (scheduler->readyQueue[i]->first->pcb->programCounter == scheduler->readyQueue[i]->first->pcb->memEnd)
            {
                // done, remove
                MLFQSRemoveTask(scheduler, i);
            }
            else
            {
                // not done, rotate if 4th, move down if not 4th AND THE REST IS NOT EMPTY, if it is the only task in the queue, then it remains on top
                if (i == 3)
                {
                    // 4th, rotate
                    // MakeReady(scheduler->readyQueue[i]->first->pcb)
                    InsertLast(scheduler->readyQueue[i], RemoveFirst(scheduler->readyQueue[i]));
                }
                else if (i != 0 || (scheduler->readyQueue[1]->size + scheduler->readyQueue[2]->size + scheduler->readyQueue[3]->size))
                {
                    // either not the first, or it is the first but everything else is non empty, so either way push down
                    // MakeReady(scheduler->readyQueue[i]->first->pcb)
                    PCB pcb = RemoveFirst(scheduler->readyQueue[i]);
                    pcb.priority++;
                    InsertLast(scheduler->readyQueue[i + 1], pcb);
                }
            }
            if (exit)
            {
                break;
            }
        }
    }
    return;
}

void MLFQSStop(MLFQ_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
}

void MLFQSBlock(MLFQ_Scheduler *scheduler, int resourceIndex, int taskIndex)
{
    resourceIndex--;
    PCB pcb = RemoveFirst(scheduler->readyQueue[taskIndex]->first);
    PriorityInsert(scheduler->blockedQueue[resourceIndex], pcb);
}

void Free(MLFQ_Scheduler *scheduler, int resourceIndex)
{
    // whatever was blocked should be replaced
    resourceIndex--;
    if (!scheduler->blockedQueue[resourceIndex]->size)
        return;
    PCB pcb = RemoveFirst(scheduler->blockedQueue[resourceIndex]);
    InsertLast(scheduler->readyQueue[pcb.priority], pcb);
}
