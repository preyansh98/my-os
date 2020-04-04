Welcome!

OS Used : Ubuntu 18.04 LTS
Testfile provided : TESTFILE.txt (all functions are tested)
Compile script : ./compile.sh will create executable named mykernel

Notes/Assumptions: 
1. Invalid "exec" commands by user will terminate program execution
but will not terminate the shell itself. 
2. Ready Queue is implemented as head and tail global pointers in kernel.c
3. Recursion on exec calling exec is not handled (as per assignment description)
