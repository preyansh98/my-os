#ifndef PCB_H
#define PCB_H
#include "constants.h"

typedef struct PCB{
    int PC;
    int pageTable[RAM_SIZE / PAGE_LENGTH]; 
    int start;
    int end;   
    struct PCB* next; 
}PCB; 

PCB* makePCB(int start, int end); 

#endif
