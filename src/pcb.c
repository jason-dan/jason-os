/****************************************************
 * pcb.c - Contains data-structures and helper      *
 * functions for the process control blocks.        *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"

void initPageTable(PCB *pcb);

PCB* makePCB(int pid, int pages_max, char *p_filename) {
    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->pid = pid;
    newPCB->PC = 0;
    newPCB->filename = p_filename;
    newPCB->PC_page = 0;
    newPCB->PC_offset = 0;
    newPCB->pages_max = pages_max;

    initPageTable(newPCB);
    return newPCB;
}

void initPageTable(PCB *pcb) {
    for (int i = 0; i < 10; i++) {
        pcb->pageTable[i] = -1;
    }
}

void PCB__destroy(PCB *pcb) {
    free(pcb->filename);
    free(pcb);
}