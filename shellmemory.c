#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"

struct MEM shellMemory[1000]; 

int setVar(char *var, char *val); 
int printVar(char *var); 
int findVarIdx(char *var, int *retIdx); 
int memFilled = 0; 

int setVar(char *var, char *val){
	int idx= 0;
       	
	if(findVarIdx(var, &idx) == 0) {
		char *var_dup = strdup(var); 
		char *val_dup = strdup(val); 
		struct MEM newVar = {var_dup, val_dup}; 
		shellMemory[memFilled++] = newVar; 	
	} else { 
		shellMemory[idx].var = strdup(var); 
		shellMemory[idx].value = strdup(val); 
	}
	 
	return 0; 
}

int printVar(char *var){
       	int idx = 0; 
	if(findVarIdx(var, &idx) == 0){
		printf("Variable does not exist \n"); 
	} else {
		printf("%s \n", shellMemory[idx].value);  
	}

	return 0; 
}

//searches shellMemory for variable and returns 0 if doesn't exist. or else it returns 1 if exists as well as saves the idx in retIdx
int findVarIdx(char *var, int *retIdx){
	if(memFilled == 0) return 0;	

	for(int i = 0; i<memFilled; i++){
		if(strcmp(shellMemory[i].var, var) == 0){
			*retIdx = i; 
			return 1; 
		}
	}

	return 0; 
}
