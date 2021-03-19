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
#include <stdbool.h>
#include "shell.h"
#include "pcb.h"
#include "readyqueue.h"
#include "kernel.h"
#include "ram.h"
#include "cpu.h"
#include "shellmemory.h"

int kernel();
void boot();

int main(int argc, char **argv) {
    int error = 0;
    boot();
    error = kernel();
    return error;
}

int myinit(char *filename) {
    int startIndex = 0, endIndex = 0;
    PCB* pcb = NULL;

    int errorCode = EXIT_SUCCESS;

    FILE *file = fopen(filename, "r");
    if (!file) errorCode = EIO;

    if (!errorCode) {
        addToRAM(file, &startIndex, &endIndex);
        if (endIndex < startIndex) {
            errorCode = ENOMEM;
            printf("ERROR: Not enough RAM to add program.\n");
        }
    }

    if (!errorCode) pcb = makePCB(startIndex, endIndex);

    if (pcb != NULL) ReadyQueue__push(pcb);

    if (file) fclose(file);
    return errorCode;
}

int scheduler() {
    PCB* pcb = ReadyQueue__pop();

    while (pcb) {
        if (Cpu__isBusy()) continue;
        
        Cpu__setIP(pcb->PC);

        int quanta = ((pcb->end - pcb->PC + 1 < DEFAULT_QUANTA) ? pcb->end - pcb->PC + 1 : DEFAULT_QUANTA);
        Cpu__run(quanta);
        pcb->PC+= quanta;

        if (pcb->PC > pcb->end) {
            Ram__clear(pcb->start, pcb->end);
            free(pcb);
        } else {
            ReadyQueue__push(pcb);
        }

        pcb = ReadyQueue__pop();        
    }

    return EXIT_SUCCESS;
}

int kernel() {
    printf("Kernel 1.0 loaded!\n");
    return shellUI();
}

void boot() {
    printf("boot not implemented\n");
}

void quit() {
    ShellMemory__cleanup();
    ReadyQueue__cleanup();
    Cpu__cleanup();
    printf("Bye!\n");
    exit(EXIT_SUCCESS);
}