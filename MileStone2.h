#ifndef MileStone2.h
#define MileStone2 .h
#include "PCB.h"

char *readFile(char *line, PCB *currentProcess);

void makeReady(PCB *pcb);

int Parse(PCB *CurrentProcess);

#endif