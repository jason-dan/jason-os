#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H

    // Initialize all global data structure
    // and variables to zero or null. 
    // Called from boot().
    void initIO();   

    // Create and format partition. Called 
    // from mount() in interpreter. Assumes that partition does not exist.
    int partition(char *name, int totalblocks, int blocksize);  

    // Open the partition from the PARTITION directory
    // and loads the information from the partition in the
    // global partition and FAT structures. This function
    // will also malloc block_size bytes and assign that to block_buffer.
    int mountFS(char *name);    

    // Find filename or creates file 
    // if it does not exist, returns file's FAT index.
    // Returns -1 on error.
    int openfile(char *name);   

    // Returns 1 if partition with name already exists, else 0.
    int partitionExists(char *name);

    // Using the file FAT index number,
    // write data to disk at current_location
    // Assumes that file is opened.
    int writeBlock(int file, char *data);

    // Reads all the blocks of the given file 
    // (represented as FAT index) and returns
    // a newly allocated string containing file contents
    char* readFile(int file);


#define MAX_BLOCK_COUNT 200
#define MAX_BLOCK_SIZE UINT_MAX
#define MAX_FILENAME_LENGTH 20
#endif
