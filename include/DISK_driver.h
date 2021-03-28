#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H
    void initIO();   // Initialize all global data structure and variables to zero or null. Called from boot().
    int partition(char *name, int blocksize, int totalblocks);  // Create and format partition. Called from mount() in interpreter.
    int mountFS(char *name);    // Load FAT & create buffer_block. Called from mount().
    int openfile(char *name);   // Find filename or creates file if it does not exist, returns file's FAT index.
#endif
