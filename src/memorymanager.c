/****************************************************
 * memorymanager.c - Responsible for launching      *
 * programs using the exec() command (not the run   *
 * command), managing frames, and paging.           *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "memorymanager.h"
#include "shell.h"
#include "pcb.h"
#include "readyqueue.h"
#include "util.h"
#include "ram.h"

typedef struct { char** data; } PAGE_T;

int copyToBackingStore(FILE *p, char *filename);
int nextPID();
int countTotalPages(FILE *bsFile);
void loadStartPages(FILE *bsFile, PCB *pcb);
FILE* seekToPage(FILE *file, int page);
char* readLine();
PAGE_T* readPagefromDisk(FILE *f, int page);
void writePageToRAM(PAGE_T *page, int frame);
void MemoryManager__init();

int MemoryManager__hasInit = 0;
int pidCounter = 0;
PCB* frameOwner[10];         // Array to keep track of which process ID the frame is assigned to. frameOwner[frameNo] = PID

int launcher(FILE *p) {
    if (!MemoryManager__hasInit) MemoryManager__init();
    int pid = nextPID(), pageCount = 0;
    int errorCode = EXIT_SUCCESS;
    char bsFilePath[MAX_FILEPATH_LENGTH];
    FILE *bsFile = NULL;
    PCB *pcb = NULL;

    snprintf(bsFilePath, sizeof(bsFilePath), "BackingStore/%d.backingstore", pid);

    errorCode = copyToBackingStore(p, bsFilePath);
    fclose(p);

    if (!errorCode) {
        bsFile = fopen(bsFilePath, "r");
        if (!bsFile) errorCode = EIO;
    }

    if (!errorCode) {
        pageCount = countTotalPages(bsFile);
        pcb = makePCB(pid, pageCount, strdup(bsFilePath));
        loadStartPages(bsFile, pcb);
    }

    if (!errorCode) {
        ReadyQueue__push(pcb);
    } else {
        PCB__destroy(pcb);
    }

    return errorCode;
}

int copyToBackingStore(FILE *p, char* filename) {
    char lineBuffer[USER_INPUT_BUFFER_SIZE];
    FILE *copy = fopen(filename, "w");
    
    if (!copy) return EIO;

    while (fgets(lineBuffer, USER_INPUT_BUFFER_SIZE, p) != NULL) {
        fputs(lineBuffer, copy);
    }

    fclose(copy);
    return EXIT_SUCCESS;
}

int nextPID() {
    int pid = pidCounter;
    pidCounter++;
    return pid;
}

int countTotalPages(FILE *bsFile) {
    int lineCount = 0;

    while (!feof(bsFile)) {
        if (fgetc(bsFile) == '\n') lineCount++;
    }

    rewind(bsFile);
    return (lineCount % 4 == 0 ? lineCount / PAGE_SIZE : lineCount / PAGE_SIZE + 1);
}

void loadStartPages(FILE *file, PCB *pcb) {
    int frameNumber = 0, victim = -1;
    int pagesToLoad = (pcb->pages_max < 2 ? pcb->pages_max : 2);
    
    for (int pageNumber = 0; pageNumber < pagesToLoad; pageNumber++) {

        frameNumber = findFrame();

        if (frameNumber == -1) {
            victim = findVictim(pcb);
            frameNumber = victim;
        }

        updatePageTable(pcb, pageNumber, frameNumber, victim);

        loadPage(pageNumber, file, frameNumber);
    }

    pcb->PC = pcb->pageTable[0] * PAGE_SIZE;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
    PAGE_T *page = readPagefromDisk(f, pageNumber);
    if (page) {
        writePageToRAM(page, frameNumber);
        free(page->data);
        free(page);
    }
}

int findFrame() {

    for (int i = 0; i < 10; i++) {
        if (frameOwner[i] == NULL) return i;
    }
    
    return -1;
}

int findVictim(PCB *p) {
    int frame = rand() % 10;
    while(frameOwner[frame]->pid == p->pid) {
        frame = (frame + 1) % 10;
    }
    return frame;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    if (victimFrame != -1) {
        PCB* victim = frameOwner[victimFrame];
        for (int i = 0; i < 10; i++) {
            if (victim->pageTable[i] == victimFrame) {
                victim->pageTable[i] = -1;
                break;
            }
        }
    }

    frameOwner[frameNumber] = p;
    p->pageTable[pageNumber] = frameNumber;

    return 0;
}

FILE* seekToPage(FILE *file, int page) {
    if (!file) return NULL;
    rewind(file);

    int curr = 0;
    int target = page * 4;

    while (!feof(file) && curr < target) {
        if (fgetc(file) == '\n') curr++;
    }

    return (curr == target ? file : NULL);
}

PAGE_T* readPagefromDisk(FILE *f, int page) {
    char buffer[USER_INPUT_BUFFER_SIZE];
    if (seekToPage(f, page) == NULL) return NULL;
    
    PAGE_T *newPage = malloc(sizeof(PAGE_T));
    newPage->data = (char**) malloc(sizeof(char*) * PAGE_SIZE);
    
    for (int i = 0; i < 4; i++) {
        if (!feof(f)) {
            fgets(buffer, USER_INPUT_BUFFER_SIZE, f);
            newPage->data[i] = strdup(buffer);
        } else {
            newPage->data[i] = NULL;
        }
    }

    return newPage;
}

void writePageToRAM(PAGE_T *page, int frame) {
    int ramIndex = frame * PAGE_SIZE;

    for (int i = 0; i < 4; i++) {
        free(Ram__access(ramIndex));
        Ram__write(page->data[i], ramIndex);
        ramIndex++;
    }
}

void MemoryManager__init() {
    srand(time(0));
    MemoryManager__hasInit = 1;
}