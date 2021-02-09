#ifndef SHELLMEMORY_H
#define SHELLMEMORY_H
    int SM_init();                  // Allocate memory for shell memory
    int SM_set(char *words[]);      // Assign or update a value of a variable. Assumes "set VAR STR"
    int SM_print(char *words[]);    // Prints out the value of VAR otherwise an error message.
    void SM_cleanup();              // Frees memory     
#endif

#define SM_MAXSIZE 1000