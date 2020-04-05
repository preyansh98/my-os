#ifndef KERNEL_H
#define KERNEL_H
#include "pcb.h"

void myinit(PCB *pcb);
int scheduler();
void freePCB();

#endif
