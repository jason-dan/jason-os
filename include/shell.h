#ifndef SHELL_H
#define SHELL_H
    char** parseInput(char []);         // Parses a line into an array of words. Returns pointer to array.
    void displayError(int error);       // Utility method to display error messages.
    void cleanupWords(char** words);    // Frees the memory of an array of words
    char *strdup(const char *str);
#endif

#define USER_INPUT_BUFFER_SIZE 1000     // Input buffer size in chars
#define MAX_WORD_COUNT 100              // Maximum number of words in an word array