/****************************************************
 * interpreter.c - The file interpreter.c contains  *
 * the interpreter() function. Each command the     *
 * interpreter() function accepts has a             *
 * corresponding function that implements the       *
 * command's functionality. Give the function the   *
 * same name as the command.                        *
 *                                                  *
 * Commands:                                        *
 *                                                  *
 * help - Displays all the commands                 *
 *                                                  * 
 * quit - Exits and terminates the shell with "Bye!"*
 *                                                  *
 * set VAR STRING - Assigns a value to shell memory *
 *                                                  *
 * print VAR - Displays the String assigned to VAR  *
 *                                                  *
 * run SCRIPT.TXT - Executes the file SCRIPT.TXT    *  
 *                                                  *
 * exec PROG1 PROG2 PROG3 - Executes up to 3        *
 * concurrent programs provided as arguments        *
 *                                                  *
 * mount NAME NUM_BLOCKS BLOCK_SIZE - Mounts        *
 * partition, creating new partition if necessary   *
 *                                                  *
 * write FILE [WORDS] - Writes word collection to   *
 * file                                             *
 *                                                  *
 * read FILE VAR - Reads file into VAR              *
 *                                                  *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "util.h"
#include "kernel.h"
#include "readyqueue.h"
#include "DISK_driver.h"

void help();        // Displays list of available commands to user
int run();          // Runs a script from a file
int exec();         // executes scripts in the kernel
int mount();        // interpreter function for mounting partitions
int write();        // writes data to files
int read();         // reads data from file into shell memory

int interpreter(char *words[]) {    // Assumes that the input is an array of words
    int errorCode = EXIT_SUCCESS;
    
    if (strcmp(words[0], "help") == 0) {            // Display list of possible commands
        help();
    } else if (strcmp(words[0], "quit") == 0) {     // Quit shell
        quit();
    } else if (strcmp(words[0], "set") == 0) {      // Store a variable in memory
        errorCode = SM__set(words);
    } else if (strcmp(words[0], "print") == 0) {    // Print variable in memory
        errorCode = SM__print(words);
    } else if (strcmp(words[0], "run") == 0) {      // Run a script
        errorCode = run(words);
    } else if (strcmp(words[0], "exec") == 0) {      // Execute programs concurrently
        errorCode = exec(words);
    } else if (strcmp(words[0], "mount") == 0) {      // Mount and create new disk partitions
        errorCode = mount(words);
    } else if (strcmp(words[0], "write") == 0) {      // Write to file
        errorCode = write(words);
    } else if (strcmp(words[0], "read") == 0) {      // Read from file into shell memory
        errorCode = read(words);
    }
    else {
        printf("Unknown command.\n");               // No matching command
    }
    
    return errorCode;
}

void help() {
    printf(
        "The following commands are supported:\n"
        "help\t\t\tDisplays all the commands\n"
        "quit\t\t\tExits the shell\n"
        "set VAR STRING\t\tAssigns a value to shell memory\n"
        "print VAR\t\tDisplays the STRING assigned to VAR\n"
        "run SCRIPT.TXT\t\tExecutes the file SCRIPT.TXT\n"
        "exec prog1 prog2 prog3\tExecutes up to 3 concurrent programs provided as arguments\n"
    );
}

int run(char **words) {
    if (!(words[1])) return EINVAL;

    FILE* file = fopen(words[1], "r");

    if (file == NULL) return ENOENT;
    char line[USER_INPUT_BUFFER_SIZE];
    
    while(fgets(line, USER_INPUT_BUFFER_SIZE - 1, file)) {
        executeLine(line);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int exec(char **words) {
    int errorCode = EXIT_SUCCESS;
    if (!(words[1])) return EINVAL;

    int scriptCounter = 1;
    while (scriptCounter <= 3 && words[scriptCounter] && errorCode == EXIT_SUCCESS) {

            errorCode = myinit(words[scriptCounter]);

        scriptCounter++;
    }

    if (errorCode == EXIT_SUCCESS) errorCode = scheduler();

    ReadyQueue__cleanup();

}

int mount() {
    printf("mount() not implemented\n");
}
int write() {
    printf("write() not implemented\n");
} 
int read() {
    printf("read() not implemented\n");
}

char** parseInput(char ui[]) {
    char tmp[200];
    char **words = malloc(sizeof(char*) * MAX_WORD_COUNT);

    int uiPtr;  // Char ptr within input
    int wordPtr;// Char ptr within word
    int w = 0;  // Word counter

    for (uiPtr = 0; ui[uiPtr] == ' ' && uiPtr < USER_INPUT_BUFFER_SIZE; uiPtr++);  // Skip spaces

    while (ui[uiPtr] != '\0' && uiPtr < USER_INPUT_BUFFER_SIZE) {

        for (wordPtr = 0; ui[uiPtr] != '\0' && ui[uiPtr]!= ' ' && ui[uiPtr]!= '\n' && uiPtr < USER_INPUT_BUFFER_SIZE; uiPtr++, wordPtr++) {
            tmp[wordPtr] = ui[uiPtr];
        }

        tmp[wordPtr] = '\0';
        words[w] = strdup(tmp);

        while ((ui[uiPtr] == ' ' || ui[uiPtr] == '\n') && uiPtr < USER_INPUT_BUFFER_SIZE) uiPtr++;
        w++;
    }

    words[w] = NULL;

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