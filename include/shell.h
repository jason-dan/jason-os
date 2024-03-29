#ifndef SHELL_H
#define SHELL_H
    int shellUI();                          // Main shell method
    void executeLine(char *line);           // External package for executing a line of command
    #define USER_INPUT_BUFFER_SIZE 1000     // Input buffer size in chars
    #define MAX_WORD_COUNT 100              // Maximum number of words in an word array
#endif