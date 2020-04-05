#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "pcb.h"
#include "kernel.h"
#include "ram.h"
#include "cpu.h"
#include "constants.h"

void myinit(PCB *pcb); 
int scheduler();
void addToReady(PCB* pcb);  
void freePCB(); 

PCB* head = NULL;
PCB* tail = NULL; 

void myinit(PCB *pcb){
    addToReady(pcb);   
}

PCB* peekHead(){
    return head; 
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

int kernel(){
    return shellUI(); 
}

void boot(){
    //prepare backing store
    char *_command = "rm -rf BackingStore && mkdir BackingStore";
    system(_command);
}

int main(){
    boot(); 
    return kernel(); 
}
