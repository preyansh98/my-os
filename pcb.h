#ifndef PCB_H
#define PCB_H
#include "constants.h"

typedef struct PCB{
    int PC;
    int pid;
    int PC_page; 
    int PC_offset; 
    int pages_max; 
    int pageTable[NUM_PAGES]; 

    //tmp
    int start;
    int end;
  
    struct PCB* next; 
}PCB; 

PCB* makePCB(int pageNo, int pageOff, int totalPages); 

#endif
