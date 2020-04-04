#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"
#include "cpu.h"
#include "ram.h"

int interpreter(char *commands[], int numargs); 
int help(char *commands[],int numargs); 
int quit(char *commands[],int numargs); 
int set(char *commands[],int numargs); 
int print(char *commands[],int numargs); 
int run_script(char *commands[],int numargs); 
int exec(char *commands[], int numargs); 

int interpreter(char *commands[], int numargs){
	int errCode = 0;
      	
        if(strcmp(commands[0], "help") == 0) errCode = help(commands, numargs);
	else if (strcmp(commands[0], "quit") == 0) errCode = quit(commands, numargs); 
	else if (strcmp(commands[0], "set") == 0) errCode = set(commands, numargs); 
	else if (strcmp(commands[0], "print") == 0) errCode = print(commands, numargs);
	else if (strcmp(commands[0], "run") == 0) errCode = run_script(commands, numargs); 
	else if (strcmp(commands[0], "exec") == 0) errCode = exec(commands, numargs);
        else return 3;

	return errCode; 	
}

int help(char *commands[], int numargs){
	if(numargs > 1) return 2; 

	printf("COMMAND : DESCRIPTION \n"); 
 	printf("help : Displays all the commands \n");
	printf("quit : Exits / terminates the shell with \"Bye!\" \n"); 
	printf("set VAR STRING : Assigns a value to the shell memory \n"); 
	printf("print VAR : Displays the STRING assigned to VAR \n"); 
	printf("run SCRIPT.TXT : Executes the file SCRIPT.TXT \n"); 	
	
	return 0;
}
int quit(char *commands[], int numargs){
	if(numargs > 1) return 2; 

	printf("Bye!\n");
	return -1; 
 }
int set (char *commands[], int numargs){
	if(numargs < 3) return 2; 

	//concatenate everything from commands[2] onwards to a new string. 
	char buf[1000]; 
	strcpy(buf, commands[2]); 

	for(int i = 3; i<numargs; i++){
		strcat(buf, " ");
		strcat(buf, commands[i]); 
	}
	return setVar(commands[1], buf); 
 }

int print(char *commands[], int numargs){
	if(numargs != 2) return 2; 

	return printVar(commands[1]);	
}

int run_script(char *commands[], int numargs){
	if(numargs != 2) return 2; 

	int errCode = 0; 
	char line[1000]; 
	FILE *p = fopen(commands[1], "rt"); 
    	if(p == NULL) {
            printf("Script not found \n");
            return 1;
	}

	while(fgets(line, 999, p)){
		line[strlen(line) - 1] = '\0';
		strtok(line, "\r"); 		 
		errCode = parse(line);	
		if(errCode != 0) {
			fclose(p); 
			if(errCode == -1) return 0; 
                        else return errCode; 
		} 
	}
	fclose(p); 
	return errCode;
}

int exec(char *commands[], int numargs){
    if(numargs < 1 || numargs > 4) return 2; 
    
    //check duplicates
    for(int ptr = 1; ptr<numargs; ptr++){
        char *prog = commands[ptr]; 
        for(int ptr1 = ptr+1; ptr1 < numargs; ptr1++){
            if(strcmp(prog, commands[ptr1]) == 0) {
                printf("Error: Script %s already loaded \n", prog);
                return 1; 
            }
        }
    }
       
    initCPU(); 
    int _loaderr = 0;

    for(int ptr = 1; ptr<numargs; ptr++){ 
        if(myinit(commands[ptr]) != 0){
            _loaderr = -1;
            break;   
        }
    }
    
    if(_loaderr == 0)
        scheduler(); 
    
    clearRAM();
    freePCB(); 

    return 0;  
}
