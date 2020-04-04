#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

PCB* makePCB(int start, int end); 

PCB* makePCB(int start, int end){
    PCB *pcb = malloc (sizeof (PCB));
    pcb->start = start; 
    pcb->end = end;
    pcb->next = NULL; 
    
    return pcb;  
}
