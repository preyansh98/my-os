#ifndef KERNEL_H
#define KERNEL_H
#include "pcb.h"

PCB* peekHead();
int myinit(PCB* pcb); 
int scheduler();
void freePCB();

#endif
