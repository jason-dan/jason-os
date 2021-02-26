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

void cleanup();

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
            free(inputBuffer);
            printf("Bye!\n");
            exit(EXIT_SUCCESS);
        }
        displayError(errorCode);
    }
}

char** parseInput(char ui[]) {
    char tmp[200];
    char **words = (char**) malloc(sizeof(char*) * MAX_WORD_COUNT);
    int a,b;      // Char index in ui and word
    int w = 0;  // Word counter

    for (a = 0; ui[a] == ' ' && a < USER_INPUT_BUFFER_SIZE; a++);  // Skip spaces

    while (ui[a] != '\0' && a < USER_INPUT_BUFFER_SIZE) {
        for (b = 0; ui[a] != '\0' && ui[a]!= ' ' && ui[a]!= '\n' && a < USER_INPUT_BUFFER_SIZE; a++, b++) {
            tmp[b] = ui[a];
        }
        tmp[b] = '\0';
        words[w] = strdup(tmp);

        while ((ui[a] == ' ' || ui[a] == '\n') && a < USER_INPUT_BUFFER_SIZE) a++;
        w++;
    }

    return words;
}

// Frees the memory of an array of words
void cleanupWords(char** words) {
    for (int i = 0; i < MAX_WORD_COUNT && words[i]; i++) {
        free(words[i]);
        words[i] = NULL;
    }
    free(words); 
    words = NULL;
}

void cleanup() {
    SM_cleanup();
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

