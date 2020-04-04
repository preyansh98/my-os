#ifndef SHELLMEM_H
#define SHELLMEM_H

struct MEM {
	char *var;
	char *value; 
};

extern struct MEM shellMemory[1000]; 
int setVar(char *var, char *val); 
int printVar(char *var); 

#endif

