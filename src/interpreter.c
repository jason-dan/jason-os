/****************************************************
 * interpreter.c - The file interpreter.c contains  *
 * the interpreter() function. Each command the     *
 * interpreter() function accepts has a             *
 * corresponding function that implements the       *
 * command's functionality. Give the function the   *
 * same name as the command. 
 * 
 * Commands:                                        *
 * help - Displays all the commands                 *
 * quit - Exits and terminates the shell with "Bye!"*
 * set VAR STRING - Assigns a value to shell memory *
 * print VAR - Displays the String assigned to VAR  *
 * run SCRIPT.TXT - Executes the file SCRIPT.TXT    *                                                 *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"

void help();        // Displays list of available commands to user
int run();          // Runs a script from a file

int interpreter(char *words[MAX_WORD_COUNT]) {    // Assumes that the input is an array of words
    int errorCode = EXIT_SUCCESS;
    
    if (strcmp(words[0], "help") == 0) {            // User requests list of possible commands
        help();
    } else if (strcmp(words[0], "quit") == 0) {     // User wishes to quit shell
        errorCode = -1;
    } else if (strcmp(words[0], "set") == 0) {      // User requests to store a variable in memory
        errorCode = SM_set(words);
    } else if (strcmp(words[0], "print") == 0) {    // User requests to print variable in memory
        errorCode = SM_print(words);
    } else if (strcmp(words[0], "run") == 0) {      // User requests to run a script
        errorCode = run(words);
    } else {
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
    );
}

int run(char **words) {
    if (!(words[1])) return EINVAL;

    FILE* file = fopen(words[1], "r");

    if (file == NULL) return ENOENT;
    char line[USER_INPUT_BUFFER_SIZE];
    
    while(fgets(line, USER_INPUT_BUFFER_SIZE - 1, file)) {
        displayError(interpreter(parseInput(line)));
    }

    fclose(file);
    return EXIT_SUCCESS;
}