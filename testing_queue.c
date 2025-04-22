#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    PCB a, b;
    a.processID = 1;
    b.processID = 2;
    a.priority = 2;
    b.priority = 3;
    PCB c, d;
    c.processID = 3;
    c.priority = 1;
    d.processID = 4;
    d.priority = 3;
    Queue *queue = CreateQueue();
    PriorityInsert(queue, a);
    printf("%d\n", queue->first->pcb->processID);
    PriorityInsert(queue, b);
    printf("%d %d\n", queue->first->pcb->processID, queue->first->next->pcb->processID);
    PriorityInsert(queue, c);
    printf("%d %d %d\n", queue->first->pcb->processID, queue->first->next->pcb->processID, queue->first->next->next->pcb->processID);
    PriorityInsert(queue, d);
    printf("%d %d %d %d\n", queue->first->pcb->processID, queue->first->next->pcb->processID, queue->first->next->next->pcb->processID, queue->last->pcb->processID);
    return 0;
}