/****************************************************
 * DISK_driver.c - Contains data-structures and     *
 * algorithms to implement disk partition creation, *
 * mounting, and file IO operations.                *
 * Jason Dan | 260718739                            *
 ****************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "DISK_driver.h"
#include "util.h"

char* getPartitionPath(char *name);

// Filepath to partition folder
const char partitionPath[] = "./PARTITION/";

// Partition file extension
const char partitionExt[] = ".partition";

// Local copy of active partition metadata
struct PARTITION {
    unsigned char total_blocks; // Total blocks for file data
    unsigned char used_blocks;  // Number of non-empty blocks
    unsigned int block_size;    // Size of each block in bytes
} localPartition;

// Local copy of active FAT
struct FAT {
    char filename[20];              // Limit is 20 chars (including null end character)
    unsigned char file_length;      // Number of data blocks affiliated with the file
    unsigned char blockPtrs[10];    // Pointers to the blocks in the parttion populated with data. These pointers are block numbers.
    signed char current_location;   // Pointer to the current read/write location of the file in block number. Initialized to -1 when not used.
} localFAT[20];

// Block buffer is malloced to the correct size 
// when the partition() function is called, with the size
// of block_size. The block buffer is used by read_block()
// to bring in blocks from disk.
char *block_buffer = NULL;

// The active file table contains all
// the system wide open file pointers,
// which is 5 maximum.
FILE *active_file_table[5] = {NULL};

// The active file map keeps track of which
// file, represented as FAT index number, is
// associated with the file pointer. The value
// set to -1 when there is no file affiliated.
int active_file_map[5];


void initIO() {

    // Initialize local partition data
    localPartition.block_size = 0;
    localPartition.total_blocks = 0;
    localPartition.used_blocks = 0;

    // Initialize all 20 entries of local FAT
    for (int i = 0; i < 20; i++) {

        // Fill filename with null
        memset(localFAT[i].filename, '\0', sizeof(localFAT[i].filename));

        localFAT[i].file_length = 0;

        for (int j = 0; j < 10; j++) {
            localFAT[i].blockPtrs[j] = 0;
        }
        
        localFAT[i].current_location = -1;
    }

    // Initialize active file map
    for (int i = 0; i < 5; i++) {
        active_file_map[i] = -1;
    }

}   

int partition(char *name, int totalblocks, int blocksize) {

    int errorCode = EXIT_SUCCESS;

    // Reset local partition and FAT structures
    initIO();

    // Populate local partition struct with params
    localPartition.block_size = (unsigned int) blocksize;
    localPartition.total_blocks = (unsigned char) totalblocks;

    // Create PARTITION folder if not present.
    if (access(partitionPath, F_OK)) {
        char command[100];
        strcpy(command, "mkdir ");
        strcat(command, partitionPath);
        system(command);
    }

    // Create partition file
    char *filepath = getPartitionPath(name);
    FILE* pfile = fopen(filepath, "wb");
    free(filepath);
    if (!pfile) return EXIT_FAILURE;

    // Write partition to file
    if (fwrite(&localPartition, sizeof(localPartition), 1, pfile) != 1) errorCode = EXIT_FAILURE;

    // Write FAT to file
    if (fwrite(&localFAT, sizeof(localFAT), 1, pfile) != 1) errorCode = EXIT_FAILURE;

    // Write (blocksize * total blocks) '0' to file.
    for (int i = 0; i < (blocksize * totalblocks); i++) {
        fputc('0', pfile);
    }

    fclose(pfile);

    // If file write was unsuccessful, then delete file and return failure.
    if (errorCode) {
        filepath = getPartitionPath(name);
        char command[100];
        strcpy(command, "rm -f ");
        strcat(command, filepath);

        system(command);
        free(filepath);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}  

int mountFS(char *name) {
    
    initIO();
    
    char *filepath = getPartitionPath(name);
    FILE *pfile = fopen(filepath, "rb");
    free(filepath);
    if (!pfile) return EXIT_FAILURE;

    // Read entire contents of partition and fat data
    fread(&localPartition, sizeof(localPartition), 1, pfile);
    fread(&localFAT, sizeof(localFAT), 1, pfile);

    fclose(pfile);

    if (block_buffer) free(block_buffer);
    block_buffer = (char*) malloc(localPartition.block_size);

    return EXIT_SUCCESS;
}

int openfile(char *name) {
    printf("openfile not implemeted\n");
}  

int partitionExists(char *name) {

    char *filepath = getPartitionPath(name);
    
    int exists = (access(filepath, F_OK) == 0) ? 1 : 0;
    free(filepath);

    return exists;
}

int writeBlock(int file, char *data) {
    printf("writeBlock not implemented\n");
}

// Appends argument partition name to partition folder path. Allocates and returns filepath to partition.
char* getPartitionPath(char *name) {
    char *filepath = (char*) malloc(sizeof(char) * 100);
    strcpy(filepath, partitionPath);
    strcat(filepath, name);
    strcat(filepath, partitionExt);
    return filepath;
}