#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H

    // Initialize all global data structure
    // and variables to zero or null. 
    // Called from boot().
    void initIO();   

    // Create and format partition. Called 
    // from mount() in interpreter. Returns 1 
    // if successful, else 0.
    int partition(char *name, int blocksize, int totalblocks);  

    // Load FAT & allocate buffer_block with partition metadata. 
    // Called from mount(). Returns 1 on success, else 0.
    int mountFS(char *name);    

    // Find filename or creates file 
    // if it does not exist, returns file's FAT index.
    int openfile(char *name);   

    // Returns 1 if partition with name already exists, else 0.
    int partitionExists(char *name);

#define MAX_BLOCK_COUNT 200
#define MAX_BLOCK_SIZE UINT_MAX
#endif
