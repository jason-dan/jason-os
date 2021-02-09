/****************************************************
 * shellmemory.c - The file shellmemory.c contains  *
 * the private data structures and public           *
 * functions that implement the shell memory.       *
 *                                                  *
 * The shell is implemented using an array          *
 * implementation of Struct MEM_ENTRY               *
 * Jason Dan | 260718739                            *
 ****************************************************/

#include "shellmemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct {
    char *var;
    char *value;
} MEM_ENTRY;

typedef struct {
    int size;
    MEM_ENTRY* array[SM_MAXSIZE];
} SHELL_MEM;

SHELL_MEM *sm;

MEM_ENTRY* SM_getEntry(char*);

int SM_init() {
    sm = NULL;
    int errorCode = EXIT_SUCCESS;

    sm = (SHELL_MEM*) malloc(sizeof(SHELL_MEM));
    if (!sm) errorCode = ENOMEM;
    return errorCode;
}

int SM_set(char *words[]) {            // Sets variable to specified string. Expects: set VAR STR
    int errorCode = EXIT_SUCCESS;
    if (!(words[1] && words[2])) return EINVAL;

    MEM_ENTRY* entry = SM_getEntry(words[1]);
    if (entry) {
        entry->value = strdup(words[2]);
    } else if (sm->size < SM_MAXSIZE) {
        sm->array[sm->size] = (MEM_ENTRY*) malloc(sizeof(MEM_ENTRY));
        sm->array[sm->size]->var = strdup(words[1]);
        sm->array[sm->size]->value = strdup(words[2]);

        sm->size = sm->size + 1;
    } else {
        errorCode = ENOMEM;
    }
    return errorCode;
}

int SM_print(char *words[]) {    // Prints out the value of VAR otherwise an error message.
    int errorCode = EXIT_SUCCESS;
    if (!(words[1])) errorCode = EINVAL;

    if (errorCode != EINVAL) {
        MEM_ENTRY* entry = SM_getEntry(words[1]);
        if (entry) {
            printf("%s\n", entry->value);
        } else {
            printf("Variable does not exist\n");
        }
    }
    return errorCode;
}   
// Searches shell memory for MEM_ENTRY containing var as variable.
// If found, returns pointer for corresponding MEM_ENTRY entry.
// If not found, returns NULL pointer
MEM_ENTRY* SM_getEntry(char *var) {
    MEM_ENTRY* entry = NULL;
    for (int i = 0; i < sm->size; i++) {
        if (strcmp(sm->array[i]->var, var) == 0) {
            entry = sm->array[i];
            break;
        }
    }
    return entry;
}

void SM_cleanup() {
    for (int i = 0; i < sm->size; i++) {
        free(sm->array[i]->var);
        free(sm->array[i]->value);
        free(sm->array[i]);
    }
    free(sm);
}