#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
    #include <stdio.h>
    #include "pcb.h"

    int launcher(FILE *p);  // Initializes and loads program into RAM, generates PCB, adds to ready queue.
    int findFrame();
    int findVictim(PCB* p);
    void loadPage(int pageNumber, FILE *f, int frameNumber);
    int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);

    #define MAX_FILEPATH_LENGTH 100
    #define PAGE_SIZE 4
#endif