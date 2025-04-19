
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
    int ProcessID;
    State ProcessState;
    int Priority;
    long ProgramCounter;
    long Mem_Bound_Up;
    long Mem_Bound_Down;
} PCB;

#endif
