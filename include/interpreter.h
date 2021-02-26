#ifndef INTERPRETER_H
#define INTERPRETER_H
    int interpreter(char *words[100]);  // Executes commands from a parsed array of words
    void cleanupWords(char** words);    // Frees the memory of an array of words
    char** parseInput(char []);         // Parses a line into an array of words. Returns pointer to array.
#endif