#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "pcb.h"
#include "kernel.h"
#include "ram.h"
#include "cpu.h"
#define CPU_QUANTA 2

int myinit(char *filename); 
int scheduler();
void addToReady(PCB* pcb);  
void freePCB(); 

PCB* head = NULL;
PCB* tail = NULL; 

void main(){
    shellUI();	
}

int myinit(char *filename){
    FILE *p = NULL; 
    
    p = fopen(filename, "rt"); 
    
    if(p == NULL) 
        return -1; 

    int startLoc = -1; 
    int endLoc = -1; 
     
    addToRAM(p, &startLoc, &endLoc); 
     
    if(startLoc == -1 || endLoc == -1)
        return -1; 
     
    PCB* pcb = makePCB(startLoc, endLoc); 
    addToReady(pcb);    
       
    return 0;   
}

int scheduler(){
      
    while(head != NULL) {
        while(isCPUAvailable() == -1) ; 

        int _ip = head->PC + head->start;
        setCPU_IP(_ip);
        
        if(_ip == head->end)
            run(CPU_QUANTA-1);
        else
            run(CPU_QUANTA);
        
        int _cpuIP = getCPU_IP(); 

        if(_cpuIP <= head->end) {
            int pcBefore = head->PC;
            head->PC = _cpuIP - head->start; 

            if(head->next == NULL) continue; 

            PCB* newHead = head->next; 
            head->next = NULL; 
            tail->next = head;
            tail = tail->next;
            tail->next = NULL; 
            head = newHead;  
        } else {
            //done. 
            PCB* newHead = head->next;
            free(head); 
            head = newHead;  
        } 
    }
    return 0; 
}

void addToReady(PCB* pcb){
    if(pcb == NULL) return; 

    if(head == NULL) {
        head = pcb; 
        tail = pcb;
    } else if(head->next == NULL){
        head->next = pcb; 
        tail = pcb; 
    } else {
        tail->next = pcb; 
        tail = pcb; 
    }
}

void freePCB(){
    while(head!=NULL){
        PCB* next = head->next; 
        free(head); 
        head = next;
    }
}
