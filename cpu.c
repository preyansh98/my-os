#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "ram.h"
#define CPU_QUANTA 2 

struct CPU {
    int IP;
    char IR[1000]; 
    int quanta; 
} cpu; 

int _cpuAvail = 1; 

int run(int quanta);
int isCPUAvailable();
int getCPU_IP(); 
void setCPU_IP(int ip); 
void initCPU();  
    
int run(int quanta){ 
    if(quanta > CPU_QUANTA) quanta = CPU_QUANTA;
    
    while(quanta > 0){
        _cpuAvail = -1;    
        int isNextInstructionValid = -1; 
        char* nextInstruction = getLineFromRAM(&isNextInstructionValid, cpu.IP); 
         
        if(isNextInstructionValid == -1){
            initCPU(); 
            break;
        } 

        strcpy(cpu.IR, nextInstruction); 
        cpu.IR[strlen(cpu.IR) - 1] = '\0';
        strtok(cpu.IR, "\r");
        
        parse(cpu.IR); 

        cpu.IP++;
        quanta--; 
    }

    _cpuAvail = 1;
    return 0; 
}

int isCPUAvailable(){
    return _cpuAvail;
}

int getCPU_IP(){
    return cpu.IP; 
}

void setCPU_IP(int ip) {
    cpu.IP = ip; 
}

void initCPU(){
    cpu.quanta = CPU_QUANTA;   
}
