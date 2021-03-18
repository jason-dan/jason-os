#ifndef RAM_H
#define RAM_H
    #include <stdio.h>
    #include <stdint.h>
    void addToRAM(FILE *p, uintptr_t *start, uintptr_t *end);
    #define RAM_SIZE 1000
#endif