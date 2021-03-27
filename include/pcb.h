#ifndef PCB_H
#define PCB_H

// #include <stdint.h>
// #include "memorymanager.h"

typedef struct {
    int pid;
    int PC;
    int PC_page;
    int PC_offset;
    int pages_max;
    char *filename;
    int pageTable[10];} PCB;

PCB* makePCB(int pid, int pages_max, char *p_filename);
void PCB__destroy(PCB* pcb);

#endif