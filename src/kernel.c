/****************************************************
 * kernel.c - This is the true home of the main()   *
 * function in the OS. The kernel.c main() function *
 * calls the shell.c int shellUI() function         *
 * (the previous main() from A1), to display the    *
 * command-line to the user. The kernel main() will *
 * also instantiate all the kernel data structures. *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"
#include "pcb.h"
#include "readyqueue.h"
#include "kernel.h"
#include "ram.h"


int myinit(char *filename) {
    int startPtr, endPtr;
    PCB* pcb = NULL;

    int errorCode = EXIT_SUCCESS;

    FILE *file = fopen(filename, "r");
    if (!file) errorCode = EIO;

    if (!errorCode) {
        addToRAM(file, &startPtr, &endPtr);
        if (endPtr < startPtr) errorCode = ENOMEM;
    }

    if (!errorCode) pcb = makePCB(startPtr, endPtr);

    if (pcb != NULL) ReadyQueue__push(pcb);

    if (file) fclose(file);
    return errorCode;
}

int scheduler() {
    printf("scheduler() not implemented");
}

int main(int argc, char **argv) {
    shellUI();
}

void Kernel__cleanup() {
    ReadyQueue__cleanup();
    printf("Kernel__cleanup() not implemented");
}

