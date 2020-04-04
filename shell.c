#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

int parse(char* userInput); 
void removeNewLine(char *userInput); 

int shellUI(){

	char prompt[100] = {'$',' ','\0'};
	char userInput[1000];
	int errorCode = 0; 

	printf("Welcome to the Preyansh Kaushik shell! \n");
	printf("Shell version 2.0 Updated February 2020 \n"); 

	while(1){
		printf("%s", prompt); 

		fgets(userInput, 999, stdin);                
                removeNewLine(userInput);

                if(strlen(userInput) == 0) continue; 

		errorCode = parse(userInput); 
		
		switch(errorCode){
			case 1: break;
			case 2: printf("Wrong number of arguments entered! \n");
				break;
			case 3: printf("Unknown command \n"); 
				break;
			case -1: exit(99);
				break;
			default: break; 
		} 
	}
	
	return 0; 
}

//parser just separates command and arguments and sends it in
int parse(char ui[]){	
	char tmp[200];
	char *words[100];

	int a, b, w = 0; 

	for(a=0; ui[a]==' ' && a<1000; a++);

	while(ui[a] != '\0' && a<1000){

		for(b=0;ui[a]!='\0' && ui[a]!=' ' && a<1000;a++,b++)
			tmp[b] = ui[a]; 

		tmp[b] = '\0';
                if(strcmp(tmp, "\0") != 0 && strcmp(tmp, "") != 0 && strcmp(tmp, " ") != 0) {      
		    words[w] = strdup(tmp);
		    w++;
                }
                a++;
	}
 
	return interpreter(words,w); 
}

void removeNewLine(char* userInput){
     if(strlen(userInput) == 0) return;
     userInput[strlen(userInput)-1] = '\0';
     strtok(userInput, "\r");
}  
