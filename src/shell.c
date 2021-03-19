/****************************************************
 * shell.c - This is the primary programming file.  *
 * It contains the main() and parse() functions.    *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"

void displayError(int error);

int shellUI() {
    int errorCode = EXIT_SUCCESS;

    char *inputBuffer = (char *) malloc(sizeof(char) * USER_INPUT_BUFFER_SIZE);

    printf("Welcome to the Jason shell!\nShell version 2.0 Updated February 2021\n");
    const char *prompt = "$";

    while (1) {
        printf("%s", prompt);

        fgets(inputBuffer, (USER_INPUT_BUFFER_SIZE - 1), stdin);

        executeLine(inputBuffer);
    }
}

void executeLine(char *line) {
    char** words = parseInput(line);
    int errorCode = interpreter(words);
    displayError(errorCode);
    cleanupWords(words);
}

// Utility method to display error messages.
void displayError(int error) {
    switch(error) {
        case EINVAL:
            printf("Missing arguments. See help for additional information.\n");
            break;
        case ENOMEM:
            printf("Memory error. Please try again \n");
            break;
        case ENOENT:
            printf("File not found.\n");
    }
}

