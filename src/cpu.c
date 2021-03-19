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

typedef struct {int IP; char IR[1000]; int quanta;} CPU_T;

void init();

CPU_T* CPU = NULL;

int Cpu__run(int quanta) {
    if (!CPU) init();
    CPU->quanta = quanta;
    
    while(CPU->quanta > 0) {
        strcpy(CPU->IR, Ram__access(CPU->IP));

        executeLine(CPU->IR);

        CPU->quanta--;
        CPU->IP++;
    }
}


void Cpu__setIP(int index) {
    if (!CPU) init();
    CPU->IP = index;
}

bool Cpu__isBusy() {
    if (!CPU) init();
    return (CPU->quanta > 0);
}

void init(){
    CPU = malloc(sizeof(CPU_T));
    CPU->IP = 0;
    CPU->quanta = 0;
}

void Cpu__cleanup() {
    free(CPU);
}