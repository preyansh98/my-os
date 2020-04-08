#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

char *ram[RAM_SIZE]; 
int k = 0; 

int findFrameIdxInRAM(int frameNo){
    return (frameNo) * PAGE_LENGTH;
}

int isFrameAvailable(int frameNo){ 
    int _f = findFrameIdxInRAM(frameNo); 
    return (_f < RAM_SIZE && ram[_f] == NULL); 
}

void setRAMCell(int cell, char *line){
    ram[cell] = strdup(line);     
}

void addToRAM(FILE *p, int *start, int *end){ 
    char line[1000]; 

    *start = k; 

    while(fgets(line, 999, p)){ 
         ram[k++] = strdup(line);
         
         if(k == RAM_SIZE) {
            *start = -1; 
            *end = -1; 
            return;
         }
    }

    *end = k-1; 
}

void clearRAM(){
    for(int i = 0; i<k; i++){
        ram[i] = NULL; 
    }
    k = 0; 
}

char* getLineFromRAM(int *isValid, int k){
    if(k < RAM_SIZE && ram[k] != NULL) {
        *isValid = 1; 
        return ram[k];
    } else {
        *isValid = -1; 
        return "";
    }
}
