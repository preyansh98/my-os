#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "constants.h"

PCB* makePCB(int pageNo, int pageOff, int totalPages){
    PCB *pcb = malloc (sizeof (PCB));
    pcb->PC_page = pageNo; 
    pcb->PC_offset = pageOff; 
    pcb->pages_max = totalPages;
    pcb->next = NULL; 

    //init pTable
    for(int i = 0; i<NUM_PAGES; ++i){
        pcb->pageTable[i] = -1; 
    }
    return pcb;  
}
