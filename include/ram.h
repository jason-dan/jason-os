#ifndef RAM_H
#define RAM_H
    #include <stdio.h>
    #include <stdint.h>
    void addToRAM(FILE *p, int *start, int *end);
    char* Ram__access(int index);
    void Ram__clear(int start, int end);
    #define RAM_SIZE 40
#endif