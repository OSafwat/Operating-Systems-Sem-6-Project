#include "PCB.h"
#include <stdlib.h>

void Copy(PCB *pcb, PCB pcbInput)
{
    pcb->memStart = pcbInput.memStart;
    pcb->memEnd = pcbInput.memEnd;
    pcb->priority = pcbInput.priority;
    pcb->processID = pcbInput.processID;
    pcb->processState = pcbInput.processState;
    pcb->programCounter = pcbInput.programCounter;
}

char *StateString(PCB *pcb)
{
    if (pcb->processState == Ready)
        return "Ready";
    if (pcb->processState == Running)
        return "Running";
    return "Blocked";
}
