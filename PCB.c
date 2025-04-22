#include "PCB.h"

void Copy(PCB *pcb, const PCB pcbInput)
{
    pcb->memStart = pcbInput.memStart;
    pcb->memEnd = pcbInput.memEnd;
    pcb->priority = pcbInput.priority;
    pcb->processID = pcbInput.processID;
    pcb->processState = pcbInput.processState;
    pcb->programCounter = pcbInput.programCounter;
}

const char *StateString(PCB *pcb)
{
    switch (*(pcb->processState))
    {
    case Ready:
        return "Ready";
    case Running:
        return "Running";
    case Blocked:
        return "Blocked";
    default:
        return "Unknown";
    }
}
