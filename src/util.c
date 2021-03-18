/****************************************************
 * util.c - Contains utility methods such as        *
 * string duplication                               *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <string.h>
#include <stdlib.h>
#include "util.h"

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = (char*) malloc(n);
    if(dup) {
        strcpy(dup, str);
    }
    return dup;
}