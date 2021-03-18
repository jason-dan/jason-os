/****************************************************
 * ram.c - Contains data-structures and helper      *
 * functions for RAM simulation.                    *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdint.h>
#include "ram.h"
#include "shell.h"
#include "util.h"

char *ram[RAM_SIZE];
int ramIndex = 0;

void addToRAM(FILE *p, int *start, int *end) {
    char line[USER_INPUT_BUFFER_SIZE];
    (*start) = ramIndex;
    
    while (ramIndex < RAM_SIZE && fgets(line, USER_INPUT_BUFFER_SIZE, p) != NULL) {
        ram[ramIndex] = strdup(line);
        ramIndex++;
    }

    (*end) = (feof(p) ? ramIndex - 1 : -1);
};


