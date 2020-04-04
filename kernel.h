#ifndef KERNEL_H
#define KERNEL_H
#include "pcb.h"

int myinit(char *filename);
int scheduler();
void freePCB();

#endif
