#ifndef PCB_H
#define PCB_H
    #include <stdint.h>
    typedef struct {int PC; int start; int end;} PCB;
    PCB* makePCB(int start, int end);
#endif