/****************************************************
 * cpu.c - Contains data-structures and algorithms  *
 * to simulate task switching.                      *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "ram.h"
#include "interpreter.h"
#include "shell.h"

typedef struct {int IP; char IR[1000]; int quanta; int offset;} CPU_T;

void Cpu__init();

CPU_T* CPU = NULL;

int Cpu__run(int quanta) {
    if (!CPU) Cpu__init();
    CPU->quanta = quanta;
    
    while(CPU->quanta > 0 && CPU->offset < 4) {
        if (Ram__access(CPU->IP + CPU->offset)) {
            strcpy(CPU->IR, Ram__access(CPU->IP + CPU->offset));

            executeLine(CPU->IR);
        }

        CPU->quanta--;
        CPU->offset++;
    }

    return quanta - CPU->quanta;
}


void Cpu__setIP(int index) {
    if (!CPU) Cpu__init();
    CPU->IP = index;
}

bool Cpu__isBusy() {
    if (!CPU) Cpu__init();
    return (CPU->quanta > 0);
}

void Cpu__setOffset(int offset) {
    if (!CPU) Cpu__init();
    CPU->offset = offset;
}

void Cpu__init(){
    CPU = malloc(sizeof(CPU_T));
    CPU->IP = 0;
    CPU->quanta = 0;
}

void Cpu__cleanup() {
    free(CPU);
}