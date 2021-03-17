#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdlib.h>
// Utility method to duplicate strings like POSIX strdup, minus the GCC errors.
// Source: https://cboard.cprogramming.com/c-programming/95462-compiler-error-warning-implicit-declaration-function-strdup.html
char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = (char*) malloc(n);
    if(dup) {
        strcpy(dup, str);
    }
    return dup;
}

#endif