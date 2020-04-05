#ifndef KERNEL_H
#define KERNEL_H
#include "pcb.h"

PCB* peekHead();
void addToReady(PCB *pcb);
int scheduler();
void freePCB();

#endif
