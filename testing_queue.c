#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    PCB a, b;
    a.processID = 1;
    b.processID = 2;
    Queue *queue = CreateQueue();
    InsertFirst(queue, a);
    printf("%d\n", queue->first->pcb->processID);
    InsertFirst(queue, b);
    int p = queue->first->pcb->processID;
    printf("%d\n%d\n", queue->first->pcb->processID, queue->first->next->pcb->processID);
    Rotate(queue);
    printf("%d %d\n", queue->first->pcb->processID, queue->last->pcb->processID);
    Rotate(queue);
    printf("%d %d", queue->first->pcb->processID, queue->last->pcb->processID);
    return 0;
}