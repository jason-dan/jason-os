#ifndef PCB_H
#define PCB_H
    #include <stdint.h>
    typedef struct {uintptr_t PC; uintptr_t start; uintptr_t end;} PCB;
    PCB* makePCB(uintptr_t start, uintptr_t end);
#endif