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

void addToRAM(FILE *p, uintptr_t *start, uintptr_t *end) {
    char line[USER_INPUT_BUFFER_SIZE];
    
    while (ramIndex < RAM_SIZE && fgets(line, USER_INPUT_BUFFER_SIZE, p) != NULL) {
        ram[ramIndex] = strdup(line);

        if (*start == 0x0) {
            (*start) = (uintptr_t) (ram[ramIndex]);
        }

        ramIndex++;
    }

    (*end) = (uintptr_t) (feof(p) ? ram[ramIndex - 1] : 0x0);
};


