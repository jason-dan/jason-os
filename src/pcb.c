/****************************************************
 * pcb.c - Contains data-structures and helper      *
 * functions for the process control blocks.        *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"

PCB* makePCB(int start, int end) {
    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->start = start;
    newPCB->PC = start;
    newPCB->end = end;
    return newPCB;
}