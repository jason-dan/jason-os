/****************************************************
 * shell.c - This is the primary UI program file.   *
 * It contains the shellUI() functions.             *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"

int shellUI() {
    const char *welcome = "Welcome to the Jason shell!\nVersion 2.0 Updated February 2021\n";
    const char *prompt = "$";

    char *inputBuffer = (char *) malloc(sizeof(char) * USER_INPUT_BUFFER_SIZE);

    printf("%s", welcome);

    while (1) {
        printf("%s", prompt);

        fgets(inputBuffer, (USER_INPUT_BUFFER_SIZE - 1), stdin);

        char** words = parseInput(inputBuffer);
        int errorCode = interpreter(words);
        cleanupWords(words);

        if (errorCode == -1) {
            break;
        }

        displayError(errorCode);
    }

    free(inputBuffer);
    printf("Bye!\n");
    return EXIT_SUCCESS;
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

