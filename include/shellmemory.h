#ifndef SHELLMEMORY_H
#define SHELLMEMORY_H
    
    int SM__set(char *words[]);      // Assign or update a value of a variable. Assumes "set VAR STR"
    int SM__print(char *words[]);    // Prints out the value of VAR otherwise an error message.
    void ShellMemory__cleanup();              // Frees memory     
#endif

#define SM_MAXSIZE 1000