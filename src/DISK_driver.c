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
#include <errno.h>

#include "DISK_driver.h"
#include "util.h"

char* getPartitionPath(char *name);

// Utility method which writes (destructive) local Partition and FAT data to partition file.
int syncPartition();

// Returns FILE pointer from active file table for provided FAT file index. Returns NULL if not currently open.
FILE* getFilePointer(int file);

// Sets provided file pointer to point to the start of specified block number
FILE* seekToBlock(FILE *filePtr, int block);

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

char *currentPartitionPath = NULL;

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

    if (currentPartitionPath) free(currentPartitionPath);

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
    
    currentPartitionPath = getPartitionPath(name);
    FILE *pfile = fopen(currentPartitionPath, "rb");
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

    int fid = -1;
    int activeFileTableIndex = -1;


    if (strlen(name) >= MAX_FILENAME_LENGTH || strlen(name) == 0) {
        printf("Error: Invalid filename. ");
        return -1;
    }

    // Check if file exists in FAT
    for (int i = 0; i < 20; i++) {
        if (strcmp(name, localFAT[i].filename) == 0) {
            fid = i;
            break;
        }
    }

    // Check if file is already opened, return if true
    if (fid != -1) {
        for (int i = 0; i < 5; i++) {
            if (active_file_map[i] == fid) return fid;
        }
    }

    // Check if capacity exists in active file table
    for (int i = 0; i < 5; i++) {
        if (active_file_table[i] == NULL) {
            activeFileTableIndex = i;
            break;
        }
    }
    if (activeFileTableIndex == -1) return -1;  // Return -1 if active file table is full

    // Create FAT entry if none exists in FAT.
    if (fid == -1) {

        // Find next available FAT cell
        for (int i = 0; i < 20; i++) {
            if (localFAT[i].filename[0] == '\0') {
                fid = i;
                break;
            }
        }
        if (fid == -1) return -1;   // Return error if no available cell in FAT

        // Create new entry in FAT
        strcpy(localFAT[fid].filename, name);
        syncPartition();
    }

    // Open file and create new entry in active file table
    active_file_table[activeFileTableIndex] = fopen(currentPartitionPath, "rb+");
    active_file_map[activeFileTableIndex] = fid;

    return fid;
}  

int partitionExists(char *name) {

    char *filepath = getPartitionPath(name);
    
    int exists = (access(filepath, F_OK) == 0) ? 1 : 0;
    free(filepath);

    return exists;
}

// Appends argument partition name to partition folder path. Allocates and returns filepath to partition.
char* getPartitionPath(char *name) {
    char *filepath = (char*) malloc(sizeof(char) * 100);
    strcpy(filepath, partitionPath);
    strcat(filepath, name);
    strcat(filepath, partitionExt);
    return filepath;
}

int syncPartition() {
    FILE *pfile = fopen(currentPartitionPath, "rb+");
    if (!pfile) {
        printf("syncPartition failed!\n");
        return EXIT_FAILURE;
    }

    fwrite(&localPartition, sizeof(localPartition), 1, pfile);
    fwrite(&localFAT, sizeof(localFAT), 1, pfile);

    fclose(pfile);
    return EXIT_SUCCESS;
}

int writeBlock(int file, char *data) {

    int errorCode = EXIT_SUCCESS;
    int posInWord = 0;  // Char index in word to write
    int wordLength = strlen(data);  // Char count of word to write
    FILE *filePtr = getFilePointer(file);  // File pointer entry in active_file_table
    
    // Check if partition has capacity
    if (localPartition.used_blocks == localPartition.total_blocks) return ENOSPC;

    // Initialize current_position if needed
    if (localFAT[file].current_location == -1) localFAT[file].current_location = 0;

    while (posInWord < wordLength && localPartition.used_blocks < localPartition.total_blocks && localFAT[file].current_location < 10) {

        // Seek file pointer to next available block
        filePtr = seekToBlock(filePtr, localPartition.used_blocks);
     
        // Update FAT block_ptr[current_location] with currently used block
        localFAT[file].blockPtrs[localFAT[file].current_location] = localPartition.used_blocks;

        // Increment used_block in partition
        localPartition.used_blocks++;

        // Fill block with word, padding with 0 if needed
        int posInBlock = 0;
        while (posInBlock < localPartition.block_size) {
            
            if (posInWord < wordLength) {
                fputc(data[posInWord], filePtr);
                posInWord++;
            } else {
                fputc('0', filePtr);
            }

            posInBlock++;
        }

        // Increment current_location
        localFAT[file].current_location++;

        // Sync partition data
        syncPartition();
    }
    fflush(filePtr);
    
    return errorCode;
}

FILE* getFilePointer(int file) {
    for (int i = 0; i < 5; i++) {
        if (active_file_map[i] == file) {
            return active_file_table[i];
        }
    }
    return NULL;
}

FILE* seekToBlock(FILE *filePtr, int block) {

    int offset = sizeof(localPartition) + sizeof(localFAT) + (localPartition.block_size * block);

    fseek(filePtr, offset, SEEK_SET);
    return filePtr;
}