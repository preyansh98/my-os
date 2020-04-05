#ifndef RAM_H
#define RAM_H

void addToRAM(FILE *p, int *start, int *end); 
int findFrameIdxInRAM(int frameNo); 
void setRAMCell(int cell, char *line);
int isFrameAvailable(int frameStart); 
char* getLineFromRAM(int *isValid, int k); 
void clearRAM();

#endif
