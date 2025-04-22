#include "queue.h"
#include "pcb.h"
#include <stdio.h>
#include <stdlib.h>

Queue *CreateQueue()
{
    Queue *queue = malloc(sizeof(Queue));
    queue->size = 0;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

void InsertFirst(Queue *queue, PCB pcb)
{
    if (!queue->size)
    {
        // empty
        Node *node = malloc(sizeof(Node));
        node->pcb = malloc(sizeof(PCB));
        Copy(node->pcb, pcb);
        queue->first = queue->last = node;
        queue->first->next = NULL;
        queue->last->next = NULL;
    }
    else
    {
        Node *node = malloc(sizeof(Node));
        node->pcb = malloc(sizeof(PCB));
        Copy(node->pcb, pcb);
        node->next = queue->first;
        queue->first = node;
    }
    queue->size++;
}

void InsertLast(Queue *queue, PCB pcb)
{
    if (!queue->size)
    {
        // empty
        Node *node = malloc(sizeof(Node));
        node->pcb = malloc(sizeof(PCB));
        Copy(node->pcb, pcb);
        queue->first = queue->last = node;
        queue->first->next = NULL;
        queue->last->next = NULL;
    }
    else
    {
        queue->last->next = malloc(sizeof(Node));
        queue->last->next->pcb = malloc(sizeof(PCB));
        Copy(queue->last->next->pcb, pcb);
        queue->last->next->next = NULL;
        queue->last = queue->last->next;
    }
    queue->size++;
}

PCB RemoveFirst(Queue *queue)
{
    Node *node = queue->first;
    PCB *pcb = node->pcb;
    queue->first = node->next;
    if (queue->first == NULL)
        queue->last = NULL;
    PCB pcbRes;
    Copy(&pcbRes, *pcb);
    free(node->pcb);
    free(node);
    queue->size--;
    return pcbRes;
}

void PriorityInsert(Queue *queue, PCB pcb)
{
    if (!queue->size)
    {
        InsertFirst(queue, pcb);
        return;
    }
    if ((queue->first->pcb->priority > pcb.priority))
    {
        InsertFirst(queue, pcb);
        return;
    }
    if ((queue->last->pcb->priority <= pcb.priority))
    {
        InsertLast(queue, pcb);
        return;
    }
    Node *node = queue->first;
    while (node->next != NULL)
    {
        if (node->next->pcb->priority > pcb.priority)
        {
            // this is where it should be inserted, after this current node
            break;
        }
        node = node->next;
    }
    Node *oldNext = node->next;
    Node *newNext = malloc(sizeof(Node));
    newNext->pcb = malloc(sizeof(PCB));
    Copy(newNext->pcb, pcb);
    node->next = newNext;
    newNext->next = oldNext;
    queue->size++;
}
