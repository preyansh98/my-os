#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "ram.h"
#include "kernel.h"
#include "constants.h"

int countTotalPages(FILE *f); 
void loadPage(int pageNumber, FILE *f, int frameNumber); 
int findFrame(); 
int findVictim(PCB *p); 
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame); 
int launcher(FILE *p);

int countTotalPages(FILE *f){
    char buf[1000]; 
    int _lines = 0; 

    while(fgets(buf,999,f))
        _lines++; 

    int numPages = _lines / PAGE_LENGTH + _lines % PAGE_LENGTH; 
    rewind(f); 
    return numPages;  
}

void loadPage(int pageNumber, FILE *f, int frameNumber){
    fseek(f, pageNumber*PAGE_LENGTH, SEEK_SET); 
    char line[1000]; 

    int _pos = findFrameIdxInRAM(frameNumber); 

    for(int i = 0; i<PAGE_LENGTH; i++){
        while(fgets(line, 999,f)){
            setRAMCell(_pos++, line);       
        }
    }

    rewind(f); 
}

int findFrame(){
    int _i = 0; 

    while ( _i < RAM_SIZE ) {
        if(isFrameAvailable(_i))
            return _i; 
        _i+=4; 
    }

    return -1; 
}

int findVictimHelper(int ptable[], int _rand){
    for(int i = 0; i < NUM_PAGES ; i++ ) 
        if(ptable[i] == _rand) return _rand; 

    return findVictimHelper(ptable, (rand() % (RAM_SIZE / PAGE_LENGTH)) + 1); 
}

int findVictim(PCB *p){
    return findVictimHelper(p->pageTable, (rand() % (RAM_SIZE / PAGE_LENGTH)) + 1); 
}

int _pid = 0; 

void updateVictimFramePCB(int victimFrame){
    //TODO!
}

int updatePageTable(PCB *pcb, int pageNumber, int frameNumber, int victimFrame){
    if(victimFrame == -1){
        pcb->pageTable[pageNumber] = frameNumber;
        return 0;
    }
    
    //victim present
    pcb->pageTable[pageNumber] = victimFrame; 
    updateVictimFramePCB(victimFrame);
    return 0;  
}

void findFrameAndLoadPage(PCB *pcb, FILE *f, int _p){
    int _f = findFrame(); 
    
    if(_f == -1){
        int _v = findVictim(pcb); 
        loadPage(_p, f, _v); 
        updatePageTable(pcb, _p, _f, _v);    
    } else {
        loadPage(_p, f, _f); 
        updatePageTable(pcb, _p, _f, -1); 
    }
}

int launcher(FILE *p){
    if(p == NULL)
        return 1; 

    char filename[100];
    snprintf(filename, 99, "BackingStore/%d.txt", ++_pid);  
    FILE *fp = fopen(filename, "wt"); 

    char line[1000];
    while(fgets(line, 999, p))
        fputs(line, fp); 

    fclose(p); 
    fclose(fp); 

    fp = fopen(filename, "rt"); 
    int totalPages = countTotalPages(fp); 
    PCB *pcb = makePCB(0,0,totalPages);
    pcb->pid = _pid; 

    findFrameAndLoadPage(pcb, fp, 1); 
    
    if(totalPages >= 2)
        findFrameAndLoadPage(pcb, fp, 2); 

    myinit(pcb); 
    return 0; 
}
