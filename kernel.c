#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "pcb.h"
#include "kernel.h"
#include "ram.h"
#include "cpu.h"
#include "memorymanager.h"
#include "constants.h"

int pageFaultInterrupt(PCB *pcb);
int myinit(PCB *pcb); 
int scheduler(); 
void addToReady(PCB *pcb); 
void moveRqHeadToEnd(); 
void freePCB(); 

PCB* head = NULL;
PCB* tail = NULL; 

int myinit(PCB *pcb){ 
    int _f = pcb->pageTable[0]; 
    pcb->PC = findFrameIdxInRAM(_f);
    addToReady(pcb);   

    return 0; 
}

PCB* peekHead(){
    return head; 
}

int scheduler(){
      
    while(head != NULL) {
        while(isCPUAvailable() == -1) ; 

        int _ip = head->PC + head->PC_offset++;
        setCPU_IP(_ip);
        
        //page fault interrupt
        if(head->PC_offset == PAGE_LENGTH)
            if(pageFaultInterrupt(head) == -1)
                return -1;

        int _err = 0; 
        if(head->PC_offset == PAGE_LENGTH - 1)
            _err = run(CPU_QUANTA-1);
        else {
            ++head->PC_offset;
            _err = run(CPU_QUANTA);
        }

        if(_err == -1) 
            if(pageFaultInterrupt(head) == -1)
                return -1; 

        if(head->PC_offset < PAGE_LENGTH) {
            if(head->next == NULL) continue; 
            moveRqHeadToEnd();  
        } else {
            //done. 
            PCB* newHead = head->next;
            free(head); 
            head = newHead;  
        } 
    }
    return 0; 
}

int pageFaultInterrupt(PCB* pcb){
    ++pcb->PC_page;
    
    if(pcb->PC_page > pcb->pages_max) return -1; 
    
    if(pcb->pageTable[pcb->PC_page] == 0) {
        char filename[100]; 
        snprintf(filename, 99, "BackingStore/swap-%d.txt", pcb->pid);
        FILE *fp = fopen(filename, "rt"); 
        findFrameAndLoadPage(pcb, fp, pcb->PC_page);
        fclose(fp); 
    }
    
    pcb->PC = findFrameIdxInRAM(pcb->pageTable[pcb->PC_page]);    
    pcb->PC_offset = 0; 

    moveRqHeadToEnd(); 
    return 0;
}

void moveRqHeadToEnd(){
     PCB* newHead = head->next; 
     head->next = NULL; 
     tail->next = head;
     tail = tail->next;
     tail->next = NULL; 
     head = newHead;  
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
