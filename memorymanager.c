#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "ram.h"
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
    fclose(f); 

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

int launcher(FILE *fp){

}
