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
#include "memorymanager.h"
#include "DISK_driver.h"

int kernel();
void boot();

int main(int argc, char **argv) {
    int error = 0;
    boot();
    error = kernel();
    return error;
}

int myinit(char *filename) {
    int errorCode = EXIT_SUCCESS;

    FILE *file = fopen(filename, "r");
    if (!file) errorCode = EIO;

    if (!errorCode) {
        errorCode = launcher(file);
    }

    if (file) fclose(file);
    return errorCode;
}

int scheduler() {
    
    PCB* pcb = ReadyQueue__pop();

    while (pcb) {
        int instructionsExecuted = 0;
        bool pageFault = false;

        if (pcb->pageTable[pcb->PC_page] != -1) {

            Cpu__setIP(pcb->PC);
            Cpu__setOffset(pcb->PC_offset);

            instructionsExecuted = Cpu__run(DEFAULT_QUANTA);

            pcb->PC_offset+=instructionsExecuted;

            // Check to see if end of page has been reached
            if (pcb->PC_offset >= PAGE_SIZE) {
                pcb->PC_offset = 0;
                pcb->PC_page++;
            }
            
            if (pcb->PC_page < pcb->pages_max) {
                if ((pcb->pageTable[pcb->PC_page] != -1) && pcb->PC_offset == 0) {
                    pcb->PC = 4 * pcb->pageTable[pcb->PC_page];
                } else if (pcb->PC_offset == 0) {
                    FILE *file = fopen(pcb->filename, "r");
                    int frame = findFrame(), victim = -1;
                    if (frame == -1) {
                        victim = findVictim(pcb);
                        frame = victim;
                    }

                    loadPage(pcb->PC_page, file, frame);
                    fclose(file);

                    updatePageTable(pcb, pcb->PC_page, frame, victim);

                    pcb->PC = frame * 4;
                
                }

                ReadyQueue__push(pcb);
            } else {
                PCB__destroy(pcb);
            }
        } else {
            // As page is invalid, load the page and put it back into the queue
            FILE *file = fopen(pcb->filename, "r");

            int frame = findFrame(), victim = -1;
            if (frame == -1) {
                victim = findVictim(pcb);
                frame = victim;
            }

            loadPage(pcb->PC_page, file, frame);
            fclose(file);

            updatePageTable(pcb, pcb->PC_page, frame, victim);

            pcb->PC = (frame * 4);

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
    system("rm -rf BackingStore");
    system("mkdir BackingStore");
    initIO();
}

void quit() {
    ShellMemory__cleanup();
    ReadyQueue__cleanup();
    Cpu__cleanup();
    printf("Bye!\n");
    exit(EXIT_SUCCESS);
}