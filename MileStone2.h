#ifndef MileStone2_H
#define MileStone2_H
#include "PCB.h"

char *readFile(char *line, PCB *currentProcess);

void makeReady(PCB *pcb);

int Parse(PCB *CurrentProcess);

#endif