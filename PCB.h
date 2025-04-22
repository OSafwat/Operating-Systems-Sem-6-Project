
#ifndef PCB_H
#define PCB_H

typedef enum
{
    Ready,
    Running,
    Blocked
} State;

typedef struct
{
    int processID;
    State *processState;
    int priority;
    long programCounter;
    long memStart;
    long memEnd;
} PCB;

char *StateString(PCB *pcb);

void Copy(PCB *pcb, PCB pcbInput);

#endif
