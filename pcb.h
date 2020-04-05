#ifndef PCB_H
#define PCB_H
#include "constants.h"

typedef struct PCB{
    int PC;
    int PC_page; 
    int PC_offset; 
    int pages_max; 
    int pageTable[RAM_SIZE / PAGE_LENGTH]; 
    
    //tmp
    int start;
    int end;

    struct PCB* next; 
}PCB; 

PCB* makePCB(int pageNo, int pageOff, int totalPages); 

#endif
