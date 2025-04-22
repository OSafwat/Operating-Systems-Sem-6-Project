#include "mlfqs.h"
#include "Queue.h"
// #include "whateverFileWillContainTheCommands"

MLFQ_Scheduler *MLFQSCreate()
{
    MLFQ_Scheduler *s = malloc(sizeof(MLFQ_Scheduler));
    for (int i = 0; i < 4; i++)
    {
        s->readyQueue[i] = CreateQueue();
        s->blockedQueue[i] = CreateQueue();
        s->timeQuantum[i] = (1 << i);
        s->currentQuantum[i] = 0;
    }
    return s;
}

void RRSInsertTask(MLFQ_Scheduler *scheduler, PCB pcb)
{
    InsertLast(scheduler->readyQueue, pcb);
}

void FCFSRemoveTask(MLFQ_Scheduler *scheduler)
{
    RemoveFirst(scheduler->readyQueue);
}

void MLFQSStart(MLFQ_Scheduler *scheduler)
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
            bool res = true;
            while (scheduler->currentlyRunning && scheduler->readyQueue[i]->first->pcb->programCounter != scheduler->readyQueue[i]->first->pcb->memEnd && scheduler->currentQuantum[i] < scheduler->timeQuantum[i])
            {
                // res = run_line(scheduler->readyQueue->first->pcb)
                if (!res)
                {
                    // should be blocked
                    MLFQSBlock(scheduler, i);
                    break;
                }
                scheduler->readyQueue[i]->first->pcb->programCounter++;
                scheduler->currentQuantum[i]++;
            }
            if (!res)
            {
                // blocked, so whatever is currently in the queue (if it is not empty) should just be started
                continue;
            }
            if (scheduler->readyQueue[i]->first->pcb->programCounter == scheduler->readyQueue[i]->first->pcb->memEnd)
            {
                // done, remove
                MLFQSRemoveTask(scheduler->readyQueue[i]);
            }
            else
            {
                // not done, rotate if 4th, move down if not 4th AND THE REST IS NOT EMPTY, if it is the only task in the queue, then it remains on top
                if (i == 3)
                {
                    // 4th, rotate
                    InsertLast(scheduler->readyQueue[i], RemoveFirst(scheduler->readyQueue[i]));
                }
                else if (i != 0 || (scheduler->readyQueue[1]->size + scheduler->readyQueue[2]->size + scheduler->readyQueue[3]->size))
                {
                    // either not the first, or it is the first but everything else is non empty, so either way push down
                    InsertLast(scheduler->readyQueue[i + 1], RemoveFirst(scheduler->readyQueue[i]));
                }
            }
        }
    }
    return;
}

void MLFQSStop(MLFQ_Scheduler *scheduler)
{
    scheduler->currentlyRunning = false;
    return;
}

void MLFQSBlock(MLFQ_Scheduler *scheduler, int index)
{
    PCB pcb = RemoveFirst(scheduler->readyQueue[index]->first);
    InsertLast(scheduler->readyQueue, pcb);
}

void FreeBlocked(MLFQ_Scheduler *scheduler)
{
    // whatever was blocked should be replaced, but inserted with priority
    for (int i = 0; i < 4; i++)
    {
        while (scheduler->blockedQueue[i]->size)
        {
            PriorityInsert(scheduler->readyQueue[i], RemoveFirst(scheduler->blockedQueue[i]->first));
        }
    }
}
