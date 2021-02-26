/****************************************************
 * shell.c - This is the primary programming file.  *
 * It contains the main() and parse() functions.    *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "util.h"

int main(int argc, char *argv[]) {
    atexit(cleanup);
    SM_init();
    
    const char *welcome = "Welcome to the Jason shell!\nVersion 1.0 Created January 2020\n";
    const char *prompt = "$";

    int errorCode = EXIT_SUCCESS;
    char *inputBuffer = (char *) malloc(sizeof(char) * USER_INPUT_BUFFER_SIZE);

    printf("%s", welcome);

    while (1) {
        printf("%s", prompt);

        fgets(inputBuffer, (USER_INPUT_BUFFER_SIZE - 1), stdin);

        char** words = parseInput(inputBuffer);
        errorCode = interpreter(words);
        cleanupWords(words);

        if (errorCode == -1) {
            SM_cleanup();
            free(inputBuffer);
            printf("Bye!\n");
            return EXIT_SUCCESS;
        }
        displayError(errorCode);
    }
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

