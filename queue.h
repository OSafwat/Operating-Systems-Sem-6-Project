#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

typedef struct Node
{
    PCB *pcb;
    struct Node *next;
} Node;

typedef struct
{
    Node *first;
    Node *last;
    int size;
} Queue;

Queue *CreateQueue(void);

void InsertFirst(Queue *queue, PCB pcb);

void InsertLast(Queue *queue, PCB pcb);

PCB RemoveFirst(Queue *queue);

void PriorityInsert(Queue *queue, PCB pcb);

#endif // FCFS_H
