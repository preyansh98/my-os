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

    int numPages = _lines / PAGE_LENGTH; 
    if(_lines % PAGE_LENGTH != 0) 
        ++numPages;
          
    rewind(f); 
    return numPages;  
}

void loadPage(int pageNumber, FILE *f, int frameNumber){
    int start = pageNumber * PAGE_LENGTH, end = start+PAGE_LENGTH; 
    
    char line[1000]; 
    int _pos = findFrameIdxInRAM(frameNumber);   
    int _counter = 0; 
    
    while(fgets(line, 999, f)){
        if(_counter == end) break;
        if(_counter++ >= start){
            setRAMCell(_pos++, line); 
        } 
    }

    rewind(f);
}

int findFrame(){
    //NUM_PAGES == NUM_FRAMES
    for (int _i = 0; _i<NUM_PAGES; ++_i){
        if(isFrameAvailable(_i))
            return _i; 
    }

    return -1; 
}

int findVictimHelper(int ptable[], int _rand){
    for(int i = 0; i < NUM_PAGES ; i++ ) 
        if(ptable[i] == _rand) 
            return findVictimHelper(ptable, (rand() % (RAM_SIZE / PAGE_LENGTH)) + 1);
    
    return _rand;  
}

int findVictim(PCB *p){
    return findVictimHelper(p->pageTable, (rand() % (RAM_SIZE / PAGE_LENGTH)) + 1); 
}

int _pid = 0; 

void updateVictimFramePCB(int victimFrame){
    PCB *rq = peekHead(); 
    if(rq == NULL) return; 
    
    int _vf = 0; 

    for( ; rq!=NULL && _vf == 0; rq=rq->next){
        for(int i = 0; i<NUM_PAGES; i++){
            if(rq->pageTable[i] == victimFrame) {
                rq->pageTable[i] = -1; 
                _vf = 1; 
                break;
            }
        }
    }
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
    snprintf(filename, 99, "BackingStore/swap-%d.txt", ++_pid);  
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

    findFrameAndLoadPage(pcb, fp, 0); 
    
    if(totalPages >= 2)
        findFrameAndLoadPage(pcb, fp,1); 

    fclose(fp);
    myinit(pcb);
    return 0; 
}
