
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
    int programCounter;
    int memStart;
    int memEnd;
} PCB;

char *stateString(PCB *pcb);

void Copy(PCB *pcb, PCB pcbInput);

#endif
