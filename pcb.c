#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

PCB* makePCB(int pageNo, int pageOff, int totalPages){
    PCB *pcb = malloc (sizeof (PCB));
    pcb->PC_page = pageNo; 
    pcb->PC_offset = pageOff; 
    pcb->pages_max = totalPages;
    pcb->next = NULL; 

    return pcb;  
}
