#ifndef PCB_H
#define PCB_H

typedef struct PCB{
    int PC;
    int start;
    int end;   
    struct PCB* next; 
}PCB; 

PCB* makePCB(int start, int end); 

#endif
